// Author:  (C) 2016 Truc Nguyen Lam
// LastChange:  Thu Aug 19

/**************************** COMPILE-TIME-OPTIONS *******************/

#include "cuddbman.h"
#include "io.h"
#include "InitFileReader.h"

extern "C" {
    extern char * cudd_stats_short(cudd_manager); // Print statictis
};

/*---------------------------------------------------------------------------*/

#include "except.h"

/*---------------------------------------------------------------------------*/

CuddBMan * CuddBMan::_instance = 0;

/*---------------------------------------------------------------------------*/
cudd_var negation (cudd_var v)
{
    return Cudd_Not(v);
}
cudd_var (*PTR_cudd_and)(cudd_manager, cudd_var, cudd_var) = 0;
cudd_var (*PTR_cudd_cofactor)(cudd_manager, cudd_var, cudd_var) = 0;
cudd_var (*PTR_cudd_exists)(cudd_manager, cudd_var, cudd_var) = 0;
cudd_var (*PTR_cudd_forall)(cudd_manager, cudd_var, cudd_var) = 0;
void (*PTR_cudd_recursive_free)(cudd_manager, cudd_var) = 0;
void (*PTR_cudd_free)(cudd_var) = 0;
cudd_manager (*PTR_cudd_init)(unsigned int, unsigned int, unsigned int, unsigned int, size_t) = 0;
cudd_var (*PTR_cudd_ite)(cudd_manager, cudd_var, cudd_var, cudd_var) = 0;
cudd_var (*PTR_cudd_new_var_last)(cudd_manager) = 0;
cudd_var (*PTR_cudd_var)(cudd_manager, int) = 0;
cudd_var (*PTR_cudd_not)(cudd_var) = &negation;
cudd_var (*PTR_cudd_one)(cudd_manager) = 0;
cudd_var (*PTR_cudd_or)(cudd_manager, cudd_var, cudd_var) = 0;
void (*PTR_cudd_quit)(cudd_manager) = 0;
cudd_var (*PTR_cudd_minimize)(cudd_manager, cudd_var, cudd_var) = 0;
cudd_var (*PTR_cudd_rel_prod)(cudd_manager, cudd_var, cudd_var, cudd_var) = 0;
cudd_var (*PTR_cudd_intersect)(cudd_manager, cudd_var, cudd_var) = 0;
int (*PTR_cudd_size)(cudd_var) = 0;
cudd_var (*PTR_cudd_compose)(cudd_manager, cudd_var, cudd_var, int) = 0;
cudd_var (*PTR_cudd_vector_compose)(cudd_manager, cudd_var, cudd_var *) = 0;
void (*PTR_cudd_unfree)(cudd_var) = 0;
cudd_var (*PTR_cudd_xnor)(cudd_manager, cudd_var, cudd_var) = 0;
cudd_var (*PTR_cudd_xor)(cudd_manager, cudd_var, cudd_var) = 0;
cudd_var (*PTR_cudd_zero)(cudd_manager) = 0;
cudd_var (*PTR_cudd_indices_to_cube)(cudd_manager, int *, int) = 0;
cudd_var (*PTR_cudd_make_prime)(cudd_manager, cudd_var, cudd_var) = 0;
int (*PTR_cudd_vars)(cudd_manager) = 0;
double (*PTR_cudd_count_min_term)(cudd_manager, cudd_var, int) = 0;


/*---------------------------------------------------------------------------*/

void CuddBMan_installAt(BooleManager ** where)
{
    verbose << "initializing CUDD Library wrapper ...\n";
    *where = CuddBMan::_instance = new CuddBMan();
}

/*---------------------------------------------------------------------------*/

extern "C" {
    void cuddbman_installAt(BooleManager ** where) { CuddBMan_installAt(where); }
};


/*---------------------------------------------------------------------------*/

CuddBMan::CuddBMan() :
    current_var(0),
    max_variables(0),
    _variables(0)
{
    load_cudd_bdd_library();
    // TODO: set variable by configuration file

    // InitFileReader initFileReader(".abcdbmanrc");

    // double factor = 0.4, MB = 30;

    // if(initFileReader.getValue("cache_ratio", factor))
    //   {
    //     verbose << "using cache ratio " << factor
    //             << " from file `.abcdbmanrc'" << '\n';
    //   }
    // else verbose << "using default cache ratio " << factor << '\n';

    // if(initFileReader.getValue("MB", MB))
    //   {
    //      verbose << "using initial memory " << MB
    //              << " MB from file `.abcdbmanrc'" << '\n';

    //     _manager = PTR_ABCD_init_MB((int) MB, (float) factor);
    //   }
    // else
    //   {
    //     verbose << "ABCD library takes (almost) all available memory";
    //     _manager = PTR_ABCD_init_take_all_Memory();
    //     verbose << " (" << PTR_ABCD_get_MB_usage(_manager) << " MB)\n";
    //   }

    _manager = PTR_cudd_init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

}
/*---------------------------------------------------------------------------*/

CuddBMan::~CuddBMan()
{
    reset_all_reprs();
    for (int i = 0; i < max_variables; i++)
        if (_variables [ i ])
        {
            // PTR_cudd_free(_variables [ i ]);
            PTR_cudd_recursive_free(manager(), _variables [ i ]);
        }

    delete _variables;

    PTR_cudd_quit(manager());
}

/*---------------------------------------------------------------------------*/

class CuddBooleRepr
    :
    public BooleRepr
{
    friend class CuddBMan;
    friend class CuddBooleSubsData;

    cudd_var cuddbdd;

public:

    CuddBooleRepr() :
        BooleRepr(CuddBMan::instance()),
        cuddbdd(0)
    {}

    ~CuddBooleRepr() { reset(); }

    cudd_manager cuddbdd_manager() { return CuddBMan::instance() -> manager(); }

    void reset()
    {
        if (cuddbdd)
        {
            PTR_cudd_recursive_free(cuddbdd_manager(), cuddbdd);
            // PTR_cudd_free(cuddbdd);
            cuddbdd = 0;
        }
    }
};

/*---------------------------------------------------------------------------
    Class CuddBooleQuantData
    Description:

*/

class CuddBooleQuantData
    :
    public BooleQuantData
{
    friend class CuddBMan;

    bool valid;
    cudd_var cuddbdd;

public:

    CuddBooleQuantData(IdxSet & s) :
        BooleQuantData(CuddBMan::instance()),
        valid(true)
    {
        cudd_var tmp = PTR_cudd_one(cuddbdd_manager());
        PTR_cudd_unfree(tmp);

        IdxSetIterator it(s);
        for (it.first(); !it.isDone(); it.next())
        {
            tmp = PTR_cudd_and(cuddbdd_manager(), tmp, PTR_cudd_var(cuddbdd_manager(), it.get()));
        }
        cuddbdd = tmp;
        PTR_cudd_unfree(cuddbdd);
        PTR_cudd_recursive_free(cuddbdd_manager(), tmp);
        // int _size = 0;
        // IdxSetIterator it(s);
        // for (it.first(); !it.isDone(); it.next()) {
        //     _size++;
        // }
        // if (_size)
        // {
        //     int * indices = new int[_size + 1];
        //     int i = 0;
        //     for (it.first(); !it.isDone(); it.next(), i++)
        //     {
        //         indices[i] = it.get();
        //     }
        //     cuddbdd = PTR_cudd_indices_to_cube(cudd_manager(), indices, _size + 1);
        //     PTR_cudd_unfree(cuddbdd);
        // }
        // else {
        //     valid = false;
        // }
    }

    ~CuddBooleQuantData() { reset(); }

    cudd_manager cuddbdd_manager() { return CuddBMan::instance() -> manager(); }
    void reset() {
        if (cuddbdd)
        {
            PTR_cudd_recursive_free(cuddbdd_manager(), cuddbdd);
            // PTR_cudd_free(cuddbdd);
            cuddbdd = 0;
        }
        valid = false;
    }
};


/*---------------------------------------------------------------------------
    CuddBooleSubsData
    This class represent ...

*/

class CuddBooleSubsData
    :
    public BooleSubsData
{
    friend class CuddBMan;
    cudd_manager cuddbdd_manager() { return CuddBMan::instance() -> manager(); }

    int * _from_ints;
    cudd_var * _to_bdds;
    int _size;

    CuddBooleSubsData(const Idx<int> & map)
        :
        BooleSubsData(CuddBMan::instance())
    {
        _size = 0;
        IdxIterator<int> it(map);
        for (it.first(); !it.isDone(); it.next())
        {
            int from = it.from(), to = it.to();
            if (from != to) _size++;
        }

        if (_size)
        {
            _from_ints = new int [ _size ];
            _to_bdds = new cudd_var [ _size ];
            int i = 0;
            for (it.first(); !it.isDone(); it.next())
            {
                int from = it.from(), to = it.to();
                if (from != to)
                {
                    _from_ints [ i ] = from;
                    cudd_var to_bdds = CuddBMan::instance() -> variables() [ to ];
                    PTR_cudd_unfree(to_bdds);
                    _to_bdds [ i ] = to_bdds;
                    i++;
                }
            }
        }
        else {
            _from_ints = 0;
            _to_bdds = 0;
        }

    }

    CuddBooleSubsData(const Idx<cudd_var> & map) :
        BooleSubsData(CuddBMan::instance())
    {
        _size = 0;
        IdxIterator<cudd_var> it(map);
        for (it.first(); !it.isDone(); it.next())
        {
            int from = it.from();
            cudd_var to = it.to();

            if (CuddBMan::instance() -> variables() [ from ] != to)
                _size++;
        }
        if (_size)
        {
            _from_ints = new int [ _size ];
            _to_bdds = new cudd_var [ _size ];
            int i = 0;
            for (it.first(); !it.isDone(); it.next())
            {
                int from = it.from();
                cudd_var to = it.to();
                PTR_cudd_unfree(to);

                if (CuddBMan::instance() -> variables() [ from ] != to)
                {
                    _from_ints [ i ] = from;
                    _to_bdds [ i ] = to;
                    i++;
                }
            }
        }
        else {
            _from_ints = 0;
            _to_bdds = 0;
        }

    }

    ~CuddBooleSubsData() {
        reset();
    }

public:

    bool valid() { return (_from_ints != 0); }
    void reset()
    {
        if (_from_ints)
        {
            delete _from_ints;
            _from_ints = 0;
        }
        if (_to_bdds)
        {
            for (int i = 0; i < _size; i++) {
                PTR_cudd_recursive_free(cuddbdd_manager(), _to_bdds [ i ]);
            }
            delete _to_bdds;
            _to_bdds = 0;
        }
    }

};



/*---------------------------------------------------------------------------*/

CuddBooleRepr *
CuddBMan::dcast(BooleRepr * br)
{
    if (br -> manager() != this) {
        error << "dynamic cast to CuddBooleRepr not implemented" << TypeViolation();
    }
    return (CuddBooleRepr *) br;
}

/*---------------------------------------------------------------------------*/

CuddBooleQuantData *
CuddBMan::dcast_quant_data(BooleQuantData * bqd)
{
    if (bqd -> manager() != this) {
        error << "dynamic cast to CuddBooleQuantData not implemented" << TypeViolation();
    }
    return (CuddBooleQuantData*) bqd;
}

/*---------------------------------------------------------------------------*/

CuddBooleSubsData *
CuddBMan::dcast_subs_data(BooleSubsData * bsd)
{
    if (bsd -> manager() != this) {
        error << "dynamic cast to CuddBooleSubsData not implemented" << TypeViolation();
    }
    return (CuddBooleSubsData*) bsd;
}

/*---------------------------------------------------------------------------*/

int
CuddBMan::new_var()
{
    if (current_var >= max_variables)
    {
        int new_max_variables = max_variables ? 2 * max_variables : 200;
        cudd_var * new_variables = new cudd_var [ new_max_variables ];

        if (max_variables > 0)
        {
            for (int i = 0; i < max_variables; i++) {
                new_variables [ i ] = _variables [ i ];
            }
            delete _variables;
        }

        for (int i = max_variables; i < new_max_variables; i++) {
            new_variables [ i ] = 0;
        }

        _variables = new_variables;
        max_variables = new_max_variables;
    }

    _variables [ current_var ] = PTR_cudd_new_var_last(manager());
    PTR_cudd_unfree(_variables [ current_var ]);
    return current_var++;
}

/*---------------------------------------------------------------------------*/

cudd_var
CuddBMan::_var(int v)
{
    ASSERT(0 <= v && v < current_var);
    cudd_var res = _variables [ v ];
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::var_to_Boole(int v)
{
    CuddBooleRepr * res = new CuddBooleRepr;
    res -> cuddbdd = _var(v);
    return res;
}


/*---------------------------------------------------------------------------*/

BooleQuantData *
CuddBMan::new_var_set(IdxSet & is)
{
    return new CuddBooleQuantData(is);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
CuddBMan::new_sub(const Idx<int> & map)
{
    return new CuddBooleSubsData(map);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
CuddBMan::new_sub(const Idx<BooleRepr*> & map)
{

    Idx<cudd_var> m;
    IdxIterator<BooleRepr*> it(map);
    for (it.first(); !it.isDone(); it.next())
    {
        cudd_var to = dcast(it.to()) -> cuddbdd;
        m.map(it.from(), to);
    }
    return new CuddBooleSubsData(m);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::_exists(BooleRepr * operand_br, BooleQuantData * bqd)
{
    CuddBooleRepr * operand = dcast(operand_br);
    CuddBooleQuantData * qd = dcast_quant_data(bqd);
    CuddBooleRepr * res = new CuddBooleRepr;

    res -> cuddbdd = PTR_cudd_exists(manager(), operand -> cuddbdd, qd -> cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::_relprod(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
    CuddBooleQuantData * qd = dcast_quant_data(bqd);
    CuddBooleRepr * a = dcast(abr), * b = dcast(bbr);
    CuddBooleRepr * res = new CuddBooleRepr;
    res -> cuddbdd = PTR_cudd_rel_prod(manager(), a -> cuddbdd, b -> cuddbdd, qd -> cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::_forall(BooleRepr * operand_br, BooleQuantData * bqd)
{
    CuddBooleRepr * operand = dcast(operand_br);
    CuddBooleQuantData * qd = dcast_quant_data(bqd);

    CuddBooleRepr * res = new CuddBooleRepr;

    /* forall E. a = !exists E. !a */
    res -> cuddbdd = PTR_cudd_forall(manager(), operand -> cuddbdd, qd -> cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::_forallImplies(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
    CuddBooleQuantData * qd = dcast_quant_data(bqd);

    CuddBooleRepr * a = dcast(abr), * b = dcast(bbr);
    CuddBooleRepr * res = new CuddBooleRepr;

    /* (forall E. A -> B) = !(exists E. A & !B) */
    res -> cuddbdd = PTR_cudd_rel_prod(manager(), a -> cuddbdd, PTR_cudd_not(b -> cuddbdd), qd -> cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    res -> cuddbdd = PTR_cudd_not(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::_forallOr(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
    CuddBooleQuantData * qd = dcast_quant_data(bqd);

    CuddBooleRepr * a = dcast(abr), * b = dcast(bbr);
    CuddBooleRepr * res = new CuddBooleRepr;

    /* (forall E. A | B) = !(exists E. !A & !B) */
    res -> cuddbdd = PTR_cudd_rel_prod(manager(), PTR_cudd_not(a -> cuddbdd), PTR_cudd_not(b -> cuddbdd), qd -> cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    res -> cuddbdd = PTR_cudd_not(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::_substitute(BooleRepr * operand_br, BooleSubsData * bsd)
{
    CuddBooleRepr * operand = dcast(operand_br);
    CuddBooleSubsData * sd = dcast_subs_data(bsd);
    CuddBooleRepr * res = new CuddBooleRepr;

    if (sd -> valid())
    {
        // res->cuddbdd = Cudd_bddSwapVariables(manager(), operand->cuddbdd, sd->_from_bdds, sd->_to_bdds, sd->_size);
        // PTR_cudd_unfree(res -> cuddbdd);
        // return res;
        int i;
        cudd_var tmp = operand->cuddbdd;
        PTR_cudd_unfree(tmp);
        for (i = 0; i < sd->_size; i++)
        {
            tmp = PTR_cudd_compose(manager(), tmp, sd->_to_bdds[i], sd->_from_ints[i]);
        }
        res->cuddbdd = tmp;
        PTR_cudd_unfree(res -> cuddbdd);
        return res;
    }
    else
    {
        res->cuddbdd = operand->cuddbdd;
        return res;
    }
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::copy(BooleRepr * br)
{
    CuddBooleRepr * a = dcast(br);
    CuddBooleRepr * res = new CuddBooleRepr;
    res -> cuddbdd = a -> cuddbdd;
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::bool_to_Boole(bool b)
{
    CuddBooleRepr *res = new CuddBooleRepr;
    res -> cuddbdd = b ? PTR_cudd_one(manager()) : PTR_cudd_zero(manager());
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::binary(BooleRepr * abr, BooleRepr * bbr, cudd_var (*f)(cudd_manager, cudd_var, cudd_var) )
{
    CuddBooleRepr * a = dcast(abr), * b = dcast(bbr);
    CuddBooleRepr * res = new CuddBooleRepr;
    res -> cuddbdd = (*f)(manager(), a -> cuddbdd, b -> cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::ite(BooleRepr * cbr, BooleRepr * tbr, BooleRepr * ebr)
{
    CuddBooleRepr * c = dcast(cbr), * t = dcast(tbr), * e = dcast(ebr);
    CuddBooleRepr * res = new CuddBooleRepr;
    res -> cuddbdd = PTR_cudd_ite(manager(), c -> cuddbdd, t -> cuddbdd, e -> cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::andop(BooleRepr * a, BooleRepr * b) { return binary(a, b, PTR_cudd_and); }

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::seilpmi(BooleRepr * a, BooleRepr * b) { return implies(b, a); }

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::implies(BooleRepr * abr, BooleRepr * bbr)
{
    CuddBooleRepr * a = dcast(abr), * b = dcast(bbr);
    CuddBooleRepr * res = new CuddBooleRepr;
    res -> cuddbdd = PTR_cudd_or(manager(), PTR_cudd_not(a -> cuddbdd), b -> cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::equiv(BooleRepr * a, BooleRepr * b) {return binary(a, b, PTR_cudd_xnor); }

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::notequiv(BooleRepr * a, BooleRepr * b) {return binary(a, b, PTR_cudd_xor); }

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::simplify_assuming(BooleRepr * a, BooleRepr * b) {return binary(a, b, PTR_cudd_minimize); }

/*---------------------------------------------------------------------------*/

BooleRepr * CuddBMan::cofactor(BooleRepr * a, BooleRepr * b) {return binary(a, b, PTR_cudd_cofactor); }

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::orop(BooleRepr * a, BooleRepr * b) { return binary(a, b, PTR_cudd_or); }

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::notop(BooleRepr * abr)
{
    CuddBooleRepr * a = dcast(abr);
    CuddBooleRepr * res = new CuddBooleRepr;
    res -> cuddbdd = PTR_cudd_not(a -> cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddBMan::onecube(BooleRepr * abr, IdxSet & set)
{
    CuddBooleRepr * a = dcast(abr);
    ASSERT(a -> cuddbdd != PTR_cudd_zero(manager()));
    CuddBooleQuantData bqd(set);
    CuddBooleRepr * res = new CuddBooleRepr;
    res -> cuddbdd = PTR_cudd_make_prime(manager(), bqd.cuddbdd, a->cuddbdd);
    PTR_cudd_unfree(res -> cuddbdd);
    return res;
}

/*---------------------------------------------------------------------------*/

float
CuddBMan::onsetsize(BooleRepr * abr, IdxSet & set)
{
    CuddBooleRepr * a = dcast(abr);
    ASSERT(PTR_cudd_vars(manager()) == current_var);

    int size_of_set = 0;
    {
        IdxSetIterator it(set);
        for (it.first(); !it.isDone(); it.next()) {
            size_of_set ++;
        }
    }
    // TODO
    double fraction = PTR_cudd_count_min_term(manager(), a -> cuddbdd, current_var);
    return (float) pow(2.0, (double) size_of_set) * fraction;
}

/*---------------------------------------------------------------------------*/

bool
CuddBMan::isTrue(BooleRepr * bbr)
{
    CuddBooleRepr * b = dcast(bbr);
    return (b -> cuddbdd == PTR_cudd_one(manager()));
}

/*---------------------------------------------------------------------------*/

bool
CuddBMan::isFalse(BooleRepr * bbr)
{
    CuddBooleRepr * b = dcast(bbr);
    return (b -> cuddbdd == PTR_cudd_zero(manager()));
}

/*---------------------------------------------------------------------------*/

bool
CuddBMan::areEqual(BooleRepr * abr, BooleRepr * bbr)
{
    CuddBooleRepr * a = dcast(abr), * b = dcast(bbr);
    return (a -> cuddbdd == b -> cuddbdd);
}

/*---------------------------------------------------------------------------*/

bool
CuddBMan::isValid(BooleRepr * br) { return dcast(br) -> cuddbdd != 0; }

/*---------------------------------------------------------------------------*/

bool
CuddBMan::doesImply(BooleRepr * abr, BooleRepr * bbr)
{
    CuddBooleRepr * a = dcast(abr), * b = dcast(bbr);
    cudd_var tmp = PTR_cudd_intersect(manager(), a->cuddbdd, PTR_cudd_not(b->cuddbdd));
    PTR_cudd_unfree(tmp);
    bool res = (tmp == PTR_cudd_zero(manager()));
    PTR_cudd_recursive_free(manager(), tmp);
    return res;
}

/*---------------------------------------------------------------------------*/

const char *
CuddBMan::stats()
{
    // TODO
    sprintf(stats_buffer, "CuddBMan: (CuddBooleRepr's)\n");
    return stats_buffer;
}

/*---------------------------------------------------------------------------*/

int
CuddBMan::size(BooleRepr * abr)
{
    CuddBooleRepr * a = dcast(abr);
    return PTR_cudd_size(a -> cuddbdd);
}

/*---------------------------------------------------------------------------*/

void
CuddBMan::visualize(BooleRepr * abr)
{
    CuddBooleRepr * a = dcast(abr);
    // TODO
}

/*---------------------------------------------------------------------------*/
