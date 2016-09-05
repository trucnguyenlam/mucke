// Author:  (C) 2016 Truc Nguyen Lam
// LastChange:  Thu Aug 28

/**************************** COMPILE-TIME-OPTIONS *******************/
#include <vector>
#include <map>

#include "cuddcppbman.h"
#include "io.h"
#include "InitFileReader.h"

extern "C" {
#include <math.h>
#include <stdio.h>
};


/*---------------------------------------------------------------------------*/

#include "except.h"

/*---------------------------------------------------------------------------*/
/* exported variables -------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int global_cudd_reduction_type = 0;

/*---------------------------------------------------------------------------*/

CuddCPPBMan * CuddCPPBMan::_instance = 0;

/*---------------------------------------------------------------------------*/

void CuddCPPBMan_installAt(BooleManager ** where)
{
    verbose << "initializing CUDD C++ Library wrapper ...\n";
    *where = CuddCPPBMan::_instance = new CuddCPPBMan();
}

/*---------------------------------------------------------------------------*/

extern "C" {
    void cuddcppbman_installAt(BooleManager ** where) { CuddCPPBMan_installAt(where); }
}

/*---------------------------------------------------------------------------*/

CuddCPPBMan::CuddCPPBMan() :
    current_var(0)
{
    InitFileReader initFileReader(".cuddbmanrc");

    double factor = 0.25;
    double cache_limit = 8388608;
    double unique_table = 1048576;

    _manager = new Cudd(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    if (initFileReader.getValue("cache_ratio", factor))
    {
        verbose << "using cache ratio " << factor
                << " from file `.cuddbmanrc'" << '\n';
    }
    else verbose << "using default cache ratio " << factor << '\n';
    _manager->SetMinHit((unsigned int) factor * 100);

    if (initFileReader.getValue("cache_limit", cache_limit))
    {
        verbose << "using hard limit for cache size " << cache_limit
                << " from file `.cuddbmanrc'" << '\n';
        _manager->SetMaxCacheHard((unsigned int) cache_limit);
    }
    else verbose << "using default cache size" << '\n';

    if (initFileReader.getValue("unique_table", unique_table))
    {
        verbose << "using limit for fast unique table growth " << unique_table
                << " from file `.cuddbmanrc'" << '\n';
        _manager->SetLooseUpTo((unsigned int) unique_table);
    }

}

/*---------------------------------------------------------------------------*/

CuddCPPBMan::~CuddCPPBMan()
{
    reset_all_reprs();
    delete _manager;
}

/*---------------------------------------------------------------------------*/

class CuddCPPBooleRepr
    :
    public BooleRepr
{
    friend class CuddCPPBMan;

    bool valid;
    BDDVar bdd;

public:

    CuddCPPBooleRepr() :
        BooleRepr(CuddCPPBMan::instance()),
        valid(true)
    {}

    ~CuddCPPBooleRepr() { reset(); }

    CUDDManager manager() { return CuddCPPBMan::instance() -> manager(); }

    void reset() {valid = false;}
};

/*---------------------------------------------------------------------------
    Class CuddCPPBooleQuantData
    Description:
*/

class CuddCPPBooleQuantData
    :
    public BooleQuantData
{
    friend class CuddCPPBMan;

    bool valid;
    BDDVar bdd;

public:

    CuddCPPBooleQuantData(IdxSet & s) :
        BooleQuantData(CuddCPPBMan::instance()),
        valid(true)
    {
        std::vector<int> indices;

        IdxSetIterator it(s);
        for (it.first(); !it.isDone(); it.next())
        {
            indices.push_back(it.get());
        }
        bdd = manager()->IndicesToCube(&indices[0], indices.size());
    }

    ~CuddCPPBooleQuantData() { reset(); }

    CUDDManager manager() { return CuddCPPBMan::instance() -> manager(); }
    void reset() {valid = false; }
};


/*---------------------------------------------------------------------------
    CuddCPPBooleSubsData
    This class represent ...
*/

class CuddCPPBooleSubsData
    :
    public BooleSubsData
{
    friend class CuddCPPBMan;
    CUDDManager manager() { return CuddCPPBMan::instance() -> manager(); }

    int _size;
    std::vector<BDDVar> subs_vector;

    CuddCPPBooleSubsData(const Idx<int> & varmap)
        :
        BooleSubsData(CuddCPPBMan::instance())
    {
        _size = 0;
        std::map<int, int> mymap;
        IdxIterator<int> it(varmap);
        for (it.first(); !it.isDone(); it.next())
        {
            int from = it.from(), to = it.to();
            if (from != to) {
                mymap[from] = to;
                _size++;
            }
        }

        if (_size)
        {
            for (int i = 0; i < manager()->ReadSize(); i++)
            {
                if (mymap.count(i) > 0)
                {
                    subs_vector.push_back(manager()->bddVar(mymap.at(i)));
                }
                else
                {
                    subs_vector.push_back(manager()->bddVar(i));
                }
            }
        }
        else
        {
        }

    }

    CuddCPPBooleSubsData(const Idx<BDDVar> & varmap) :
        BooleSubsData(CuddCPPBMan::instance())
    {
        _size = 0;
        std::map<int, BDDVar> mymap;
        IdxIterator<BDDVar> it(varmap);
        for (it.first(); !it.isDone(); it.next())
        {
            int from = it.from();
            BDDVar to = it.to();
            if (manager()->bddVar( from ) != to)
            {
                mymap[from] = to;
                _size++;
            }
        }
        if (_size)
        {
            for (int i = 0; i < manager()->ReadSize(); i++)
            {
                if (mymap.count(i) > 0)
                {
                    subs_vector.push_back(mymap.at(i));
                }
                else
                {
                    subs_vector.push_back(manager()->bddVar(i));
                }
            }
        }
        else {
        }

    }

    ~CuddCPPBooleSubsData() {
        reset();
    }

public:
    bool valid() {return (_size > 0);}
    void reset() {
        subs_vector.clear();
        _size = 0;
    }
};



/*---------------------------------------------------------------------------*/

CuddCPPBooleRepr *
CuddCPPBMan::dcast(BooleRepr * br)
{
    if (br -> manager() != this) {
        error   << "dynamic cast to CuddCPPBooleRepr not implemented"
                << TypeViolation();
    }
    return (CuddCPPBooleRepr *) br;
}

/*---------------------------------------------------------------------------*/

CuddCPPBooleQuantData *
CuddCPPBMan::dcast_quant_data(BooleQuantData * bqd)
{
    if (bqd -> manager() != this) {
        error   << "dynamic cast to CuddCPPBooleQuantData not implemented"
                << TypeViolation();
    }
    return (CuddCPPBooleQuantData*) bqd;
}

/*---------------------------------------------------------------------------*/

CuddCPPBooleSubsData *
CuddCPPBMan::dcast_subs_data(BooleSubsData * bsd)
{
    if (bsd -> manager() != this) {
        error << "dynamic cast to CuddCPPBooleSubsData not implemented"
              << TypeViolation();
    }
    return (CuddCPPBooleSubsData*) bsd;
}

/*---------------------------------------------------------------------------*/

int
CuddCPPBMan::new_var()
{
    return current_var++;
}


/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::var_to_Boole(int v)
{
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = manager()->bddVar(v);
    return res;
}


/*---------------------------------------------------------------------------*/

BooleQuantData *
CuddCPPBMan::new_var_set(IdxSet & is)
{
    return new CuddCPPBooleQuantData(is);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
CuddCPPBMan::new_sub(const Idx<int> & map)
{
    return new CuddCPPBooleSubsData(map);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
CuddCPPBMan::new_sub(const Idx<BooleRepr*> & map)
{

    Idx<BDDVar> m;
    IdxIterator<BooleRepr*> it(map);
    for (it.first(); !it.isDone(); it.next())
    {
        BDDVar to = dcast(it.to()) -> bdd;
        m.map(it.from(), to);
    }
    return new CuddCPPBooleSubsData(m);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::_exists(BooleRepr * operand_br, BooleQuantData * bqd)
{
    CuddCPPBooleRepr * operand = dcast(operand_br);
    CuddCPPBooleQuantData * qd = dcast_quant_data(bqd);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;

    res -> bdd = operand -> bdd.ExistAbstract(qd -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::_relprod(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
    CuddCPPBooleQuantData * qd = dcast_quant_data(bqd);
    CuddCPPBooleRepr * a = dcast(abr),
                       * b = dcast(bbr);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = a -> bdd.AndAbstract(b -> bdd, qd -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::_forall(BooleRepr * operand_br, BooleQuantData * bqd)
{
    CuddCPPBooleRepr * operand = dcast(operand_br);
    CuddCPPBooleQuantData * qd = dcast_quant_data(bqd);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;

    /* forall E. a = !exists E. !a */
    res -> bdd = operand -> bdd.UnivAbstract(qd -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::_forallImplies(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
    CuddCPPBooleQuantData * qd = dcast_quant_data(bqd);

    CuddCPPBooleRepr    * a = dcast(abr),
                          * b = dcast(bbr);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;

    /* (forall E. A -> B) = !(exists E. A & !B) */
    res -> bdd = !(a -> bdd.AndAbstract(!(b -> bdd), qd -> bdd));
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::_forallOr(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
    CuddCPPBooleQuantData * qd = dcast_quant_data(bqd);

    CuddCPPBooleRepr * a = dcast(abr), * b = dcast(bbr);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;

    /* (forall E. A | B) = !(exists E. !A & !B) */
    BDDVar tmp = !(a -> bdd);
    res -> bdd = tmp.AndAbstract(!(b -> bdd), qd -> bdd);
    res -> bdd = !(res -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::_substitute(BooleRepr * operand_br, BooleSubsData * bsd)
{
    CuddCPPBooleRepr * operand = dcast(operand_br);
    CuddCPPBooleSubsData * sd = dcast_subs_data(bsd);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;

    if (sd -> valid())
    {
        res->bdd = operand->bdd.VectorCompose(sd->subs_vector);
        return res;
    }
    else
    {
        res->bdd = operand->bdd;
        return res;
    }
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::copy(BooleRepr * br)
{
    CuddCPPBooleRepr * a = dcast(br);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = a -> bdd;
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::bool_to_Boole(bool b)
{
    CuddCPPBooleRepr *res = new CuddCPPBooleRepr;
    res -> bdd = b ? manager()->bddOne() : manager()->bddZero();
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::ite(BooleRepr * cbr, BooleRepr * tbr, BooleRepr * ebr)
{
    CuddCPPBooleRepr * c = dcast(cbr), * t = dcast(tbr), * e = dcast(ebr);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = c -> bdd.Ite(t -> bdd, e -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::andop(BooleRepr * a, BooleRepr * b)
{
    CuddCPPBooleRepr * abr = dcast(a),
                       * bbr = dcast(b);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = (abr -> bdd) * (bbr -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::seilpmi(BooleRepr * a, BooleRepr * b)
{
    return implies(b, a);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::implies(BooleRepr * abr, BooleRepr * bbr)
{
    CuddCPPBooleRepr * a = dcast(abr), * b = dcast(bbr);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = b -> bdd + !(a -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::equiv(BooleRepr * a, BooleRepr * b)
{
    CuddCPPBooleRepr * abr = dcast(a),
                       * bbr = dcast(b);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = abr -> bdd.Xnor(bbr -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::notequiv(BooleRepr * a, BooleRepr * b)
{
    CuddCPPBooleRepr * abr = dcast(a),
                       * bbr = dcast(b);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = abr -> bdd.Xor(bbr -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::simplify_assuming(BooleRepr * a, BooleRepr * b)
{
    CuddCPPBooleRepr * abr = dcast(a),
                       * bbr = dcast(b);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;

    if (global_cudd_reduction_type == 0)
    {
        res -> bdd = abr -> bdd.Constrain(bbr -> bdd);
    }
    else if (global_cudd_reduction_type == 1)
    {
        res -> bdd = abr -> bdd.Restrict(bbr -> bdd);
    }
    else
    {
        res -> bdd = abr -> bdd.LICompaction(bbr -> bdd);
    }

    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr * CuddCPPBMan::cofactor(BooleRepr * a, BooleRepr * b)
{
    CuddCPPBooleRepr * abr = dcast(a),
                       * bbr = dcast(b);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = abr -> bdd.Cofactor(bbr -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::orop(BooleRepr * a, BooleRepr * b)
{
    CuddCPPBooleRepr * abr = dcast(a),
                       * bbr = dcast(b);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = abr -> bdd + bbr -> bdd;
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::notop(BooleRepr * abr)
{
    CuddCPPBooleRepr * a = dcast(abr);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = !(a -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CuddCPPBMan::onecube(BooleRepr * abr, IdxSet & set)
{
    CuddCPPBooleRepr * a = dcast(abr);
    ASSERT(a -> bdd != manager()->bddZero());
    CuddCPPBooleQuantData bqd(set);
    CuddCPPBooleRepr * res = new CuddCPPBooleRepr;
    res -> bdd = a->bdd.MakePrime(bqd.bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

float
CuddCPPBMan::onsetsize(BooleRepr * abr, IdxSet & set)
{
    CuddCPPBooleRepr * a = dcast(abr);
    ASSERT(manager()->ReadSize() == current_var);

    int size_of_set = 0;
    IdxSetIterator it(set);
    for (it.first(); !it.isDone(); it.next()) {
        size_of_set ++;
    }
    double fraction = a->bdd.CountPath() / (2 * pow(2.0, (double) size_of_set)) ;
    return (float) pow(2.0, (double) size_of_set) * fraction;
}

/*---------------------------------------------------------------------------*/

bool
CuddCPPBMan::isTrue(BooleRepr * bbr)
{
    CuddCPPBooleRepr * b = dcast(bbr);
    return (b -> bdd == manager()->bddOne());
}

/*---------------------------------------------------------------------------*/

bool
CuddCPPBMan::isFalse(BooleRepr * bbr)
{
    CuddCPPBooleRepr * b = dcast(bbr);
    return (b -> bdd == manager()->bddZero());
}

/*---------------------------------------------------------------------------*/

bool
CuddCPPBMan::areEqual(BooleRepr * abr, BooleRepr * bbr)
{
    CuddCPPBooleRepr * a = dcast(abr), * b = dcast(bbr);
    return (a -> bdd == b -> bdd);
}

/*---------------------------------------------------------------------------*/

bool
CuddCPPBMan::isValid(BooleRepr * br) { return dcast(br) -> valid; }

/*---------------------------------------------------------------------------*/

bool
CuddCPPBMan::doesImply(BooleRepr * abr, BooleRepr * bbr)
{
    CuddCPPBooleRepr * a = dcast(abr),
                       * b = dcast(bbr);
    BDDVar tmp = a->bdd.Intersect(!(b->bdd));
    bool res = (tmp == manager()->bddZero());
    return res;
}

/*---------------------------------------------------------------------------*/

const char *
CuddCPPBMan::stats()
{
    char stats[ 1500 ];
    sprintf(stats,
            "CUDD: t%u n%u c%u u%.0f s%.0f h%.0f (%.0f%%) gc%d",
            manager() -> ReadSlots(),
            manager() -> ReadKeys(),
            manager() -> ReadCacheSlots(),
            manager() -> ReadCacheUsedSlots() * 100.0,
            manager() -> ReadCacheLookUps(),
            manager() -> ReadCacheHits(),
            (manager() -> ReadCacheHits() / manager() -> ReadCacheLookUps()) * 100.0,
            manager() -> ReadGarbageCollections());

    sprintf(stats_buffer, "CuddCPPBMan: %d (CuddCPPBooleRepr's)\n%s", num_reprs, stats);
    return stats_buffer;
}

/*---------------------------------------------------------------------------*/

int
CuddCPPBMan::size(BooleRepr * abr)
{
    CuddCPPBooleRepr * a = dcast(abr);
    return a -> bdd.nodeCount();
}

/*---------------------------------------------------------------------------*/

void
CuddCPPBMan::visualize(BooleRepr * abr)
{
    CuddCPPBooleRepr * a = dcast(abr);
    char buffer[200];
    sprintf(buffer, "cudd_bdd%lu.dot", (size_t) abr);
    FILE *outfile;
    outfile = fopen(buffer, "w");
    std::vector<BDDVar> v;
    v.push_back(a->bdd);
    manager()->DumpDot(v, 0, 0, outfile);
    fclose(outfile);
}

// void
// CuddCPPBMan::dump(BooleRepr * abr, void (*print)(char*), char ** names)
// {
//     // CuddCPPBooleRepr * a = dcast(abr);
//     // FILE * outfile;
//     // outfile = fopen(names)
//     warning << "dump not implemented\n";
// }

/*---------------------------------------------------------------------------*/
