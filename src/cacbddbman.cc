// Author:  (C) 2016 Truc Nguyen Lam
// LastChange:  Sat Sep 3

/**************************** COMPILE-TIME-OPTIONS *******************/
#include <vector>
#include <map>

#include "cacbddbman.h"
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

/*---------------------------------------------------------------------------*/

CacBDDBMan * CacBDDBMan::_instance = 0;

/*---------------------------------------------------------------------------*/

void CacBDDBMan_installAt(BooleManager ** where)
{
    verbose << "initializing CacBDD Library wrapper ...\n";
    *where = CacBDDBMan::_instance = new CacBDDBMan();
}

/*---------------------------------------------------------------------------*/

extern "C" {
    void cacbddbman_installAt(BooleManager ** where) { CacBDDBMan_installAt(where); }
}

/*---------------------------------------------------------------------------*/

CacBDDBMan::CacBDDBMan() :
    current_var(0)
{
    _manager = new cacBDD::XBDDManager();

}

/*---------------------------------------------------------------------------*/

CacBDDBMan::~CacBDDBMan()
{
    reset_all_reprs();
    delete _manager;
}

/*---------------------------------------------------------------------------*/

class CacBDDBooleRepr
    :
    public BooleRepr
{
    friend class CacBDDBMan;

    bool valid;
    cacVar bdd;

public:

    CacBDDBooleRepr() :
        BooleRepr(CacBDDBMan::instance()),
        valid(true)
    {}

    ~CacBDDBooleRepr() { reset(); }

    CacBDDManager manager() { return CacBDDBMan::instance() -> manager(); }

    void reset() {valid = false;}
};

/*---------------------------------------------------------------------------
    Class CacBDDBooleQuantData
    Description:
*/

class CacBDDBooleQuantData
    :
    public BooleQuantData
{
    friend class CacBDDBMan;

    bool valid;
    cacVar bdd;

public:

    CacBDDBooleQuantData(IdxSet & s) :
        BooleQuantData(CacBDDBMan::instance()),
        valid(true)
    {
        bdd = manager()->BddOne();
        IdxSetIterator it(s);
        for (it.first(); !it.isDone(); it.next())
        {
            bdd = bdd * manager()->BddVar(it.get());
        }
    }

    ~CacBDDBooleQuantData() { reset(); }

    CacBDDManager manager() { return CacBDDBMan::instance() -> manager(); }
    void reset() {valid = false; }
};


/*---------------------------------------------------------------------------
    CacBDDBooleSubsData
    This class represent ...
*/

class CacBDDBooleSubsData
    :
    public BooleSubsData
{
    friend class CacBDDBMan;
    CacBDDManager manager() { return CacBDDBMan::instance() -> manager(); }

    int _size;
    // std::vector<cacVar> subs_vector;
    std::map<int, cacVar> mymap;

    CacBDDBooleSubsData(const Idx<int> & varmap)
        :
        BooleSubsData(CacBDDBMan::instance())
    {
        _size = 0;
        // std::map<int, int> mymap;
        IdxIterator<int> it(varmap);
        for (it.first(); !it.isDone(); it.next())
        {
            int from = it.from(), to = it.to();
            if (from != to) {
                mymap[from] = manager()->BddVar(to);
                _size++;
            }
        }

    }

    CacBDDBooleSubsData(const Idx<cacVar> & varmap) :
        BooleSubsData(CacBDDBMan::instance())
    {
        _size = 0;
        // std::map<int, cacVar> mymap;
        IdxIterator<cacVar> it(varmap);
        for (it.first(); !it.isDone(); it.next())
        {
            int from = it.from();
            cacVar to = it.to();
            if (manager()->BddVar( from ) != to)
            {
                mymap[from] = to;
                _size++;
            }
        }
    }

    ~CacBDDBooleSubsData() {
        reset();
    }

public:
    bool valid() {return (_size > 0);}
    void reset() {
        // subs_vector.clear();
        mymap.clear();
        _size = 0;
    }
};



/*---------------------------------------------------------------------------*/

CacBDDBooleRepr *
CacBDDBMan::dcast(BooleRepr * br)
{
    if (br -> manager() != this) {
        error   << "dynamic cast to CacBDDBooleRepr not implemented"
                << TypeViolation();
    }
    return (CacBDDBooleRepr *) br;
}

/*---------------------------------------------------------------------------*/

CacBDDBooleQuantData *
CacBDDBMan::dcast_quant_data(BooleQuantData * bqd)
{
    if (bqd -> manager() != this) {
        error   << "dynamic cast to CacBDDBooleQuantData not implemented"
                << TypeViolation();
    }
    return (CacBDDBooleQuantData*) bqd;
}

/*---------------------------------------------------------------------------*/

CacBDDBooleSubsData *
CacBDDBMan::dcast_subs_data(BooleSubsData * bsd)
{
    if (bsd -> manager() != this) {
        error << "dynamic cast to CacBDDBooleSubsData not implemented"
              << TypeViolation();
    }
    return (CacBDDBooleSubsData*) bsd;
}

/*---------------------------------------------------------------------------*/

int
CacBDDBMan::new_var()
{
    return current_var++;
}


/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::var_to_Boole(int v)
{
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = manager()->BddVar(v);
    return res;
}


/*---------------------------------------------------------------------------*/

BooleQuantData *
CacBDDBMan::new_var_set(IdxSet & is)
{
    return new CacBDDBooleQuantData(is);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
CacBDDBMan::new_sub(const Idx<int> & map)
{
    return new CacBDDBooleSubsData(map);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
CacBDDBMan::new_sub(const Idx<BooleRepr*> & map)
{

    Idx<cacVar> m;
    IdxIterator<BooleRepr*> it(map);
    for (it.first(); !it.isDone(); it.next())
    {
        cacVar to = dcast(it.to()) -> bdd;
        m.map(it.from(), to);
    }
    return new CacBDDBooleSubsData(m);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::_exists(BooleRepr * operand_br, BooleQuantData * bqd)
{
    CacBDDBooleRepr * operand = dcast(operand_br);
    CacBDDBooleQuantData * qd = dcast_quant_data(bqd);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;

    res -> bdd = operand -> bdd.Exist(qd -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::_relprod(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
    CacBDDBooleQuantData * qd = dcast_quant_data(bqd);
    CacBDDBooleRepr * a = dcast(abr),
                      * b = dcast(bbr);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = a -> bdd.AndExist(b -> bdd, qd -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::_forall(BooleRepr * operand_br, BooleQuantData * bqd)
{
    CacBDDBooleRepr * operand = dcast(operand_br);
    CacBDDBooleQuantData * qd = dcast_quant_data(bqd);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;

    /* forall E. a = !exists E. !a */
    res -> bdd = operand -> bdd.Universal(qd -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::_forallImplies(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
    CacBDDBooleQuantData * qd = dcast_quant_data(bqd);

    CacBDDBooleRepr    * a = dcast(abr),
                         * b = dcast(bbr);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;

    /* (forall E. A -> B) = !(exists E. A & !B) */
    res -> bdd = !(a -> bdd.AndExist(!(b -> bdd), qd -> bdd));
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::_forallOr(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
    CacBDDBooleQuantData * qd = dcast_quant_data(bqd);

    CacBDDBooleRepr * a = dcast(abr), * b = dcast(bbr);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;

    /* (forall E. A | B) = !(exists E. !A & !B) */
    cacVar tmp = !(a -> bdd);
    res -> bdd = tmp.AndExist(!(b -> bdd), qd -> bdd);
    res -> bdd = !(res -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::_substitute(BooleRepr * operand_br, BooleSubsData * bsd)
{
    CacBDDBooleRepr * operand = dcast(operand_br);
    CacBDDBooleSubsData * sd = dcast_subs_data(bsd);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;

    if (sd -> valid())
    {
        res->bdd = operand->bdd;
        for (std::map<int, cacVar>::iterator it = sd->mymap.begin(); it != sd->mymap.end(); ++it)
        {
            res ->bdd = res->bdd.Compose(it->first, it->second);
        }
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
CacBDDBMan::copy(BooleRepr * br)
{
    CacBDDBooleRepr * a = dcast(br);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = a -> bdd;
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::bool_to_Boole(bool b)
{
    CacBDDBooleRepr *res = new CacBDDBooleRepr;
    res -> bdd = b ? manager()->BddOne() : manager()->BddZero();
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::ite(BooleRepr * cbr, BooleRepr * tbr, BooleRepr * ebr)
{
    CacBDDBooleRepr * c = dcast(cbr), * t = dcast(tbr), * e = dcast(ebr);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = manager()->Ite(c -> bdd, t -> bdd, e -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::andop(BooleRepr * a, BooleRepr * b)
{
    CacBDDBooleRepr * abr = dcast(a),
                      * bbr = dcast(b);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = (abr -> bdd) * (bbr -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::seilpmi(BooleRepr * a, BooleRepr * b)
{
    return implies(b, a);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::implies(BooleRepr * abr, BooleRepr * bbr)
{
    CacBDDBooleRepr * a = dcast(abr), * b = dcast(bbr);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = b -> bdd + !(a -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::equiv(BooleRepr * a, BooleRepr * b)
{
    CacBDDBooleRepr * abr = dcast(a),
                      * bbr = dcast(b);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = abr -> bdd & bbr -> bdd;
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::notequiv(BooleRepr * a, BooleRepr * b)
{
    CacBDDBooleRepr * abr = dcast(a),
                      * bbr = dcast(b);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = abr -> bdd ^ bbr -> bdd;
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::simplify_assuming(BooleRepr * a, BooleRepr * b)
{
    CacBDDBooleRepr * abr = dcast(a),
                      * bbr = dcast(b);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;

    res -> bdd = abr -> bdd.Restrict(bbr -> bdd);

    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr * CacBDDBMan::cofactor(BooleRepr * a, BooleRepr * b)
{
    CacBDDBooleRepr * abr = dcast(a),
                      * bbr = dcast(b);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = abr -> bdd.Restrict(bbr -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::orop(BooleRepr * a, BooleRepr * b)
{
    CacBDDBooleRepr * abr = dcast(a),
                      * bbr = dcast(b);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = abr -> bdd + bbr -> bdd;
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::notop(BooleRepr * abr)
{
    CacBDDBooleRepr * a = dcast(abr);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    res -> bdd = !(a -> bdd);
    return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
CacBDDBMan::onecube(BooleRepr * abr, IdxSet & set)
{
    CacBDDBooleRepr * a = dcast(abr);
    ASSERT(a -> bdd != manager()->BddZero());
    CacBDDBooleQuantData bqd(set);
    CacBDDBooleRepr * res = new CacBDDBooleRepr;
    // res -> bdd = a->bdd.MakePrime(bqd.bdd);
    warning << "onecube not implemented\n";

    return res;
}

/*---------------------------------------------------------------------------*/

float
CacBDDBMan::onsetsize(BooleRepr * abr, IdxSet & set)
{
    // CacBDDBooleRepr * a = dcast(abr);
    // ASSERT(manager()->ReadSize() == current_var);
    warning << "onsetsize not implemented\n";

    int size_of_set = 0;
    IdxSetIterator it(set);
    for (it.first(); !it.isDone(); it.next()) {
        size_of_set ++;
    }
    // double fraction = a->bdd.CountPath() / (2 * pow(2.0, (double) size_of_set)) ;
    double fraction = 1.0;
    return (float) pow(2.0, (double) size_of_set) * fraction;
}

/*---------------------------------------------------------------------------*/

bool
CacBDDBMan::isTrue(BooleRepr * bbr)
{
    CacBDDBooleRepr * b = dcast(bbr);
    return (b -> bdd == manager()->BddOne());
}

/*---------------------------------------------------------------------------*/

bool
CacBDDBMan::isFalse(BooleRepr * bbr)
{
    CacBDDBooleRepr * b = dcast(bbr);
    return (b -> bdd == manager()->BddZero());
}

/*---------------------------------------------------------------------------*/

bool
CacBDDBMan::areEqual(BooleRepr * abr, BooleRepr * bbr)
{
    CacBDDBooleRepr * a = dcast(abr), * b = dcast(bbr);
    return (a -> bdd == b -> bdd);
}

/*---------------------------------------------------------------------------*/

bool
CacBDDBMan::isValid(BooleRepr * br) { return dcast(br) -> valid; }

/*---------------------------------------------------------------------------*/

bool
CacBDDBMan::doesImply(BooleRepr * abr, BooleRepr * bbr)
{
    CacBDDBooleRepr * a = dcast(abr),
                      * b = dcast(bbr);
    cacVar tmp = a->bdd * (!(b->bdd));
    bool res = (tmp == manager()->BddZero());
    return res;
}

/*---------------------------------------------------------------------------*/

const char *
CacBDDBMan::stats()
{
    // TODO
    // char stats[ 1500 ];
    // sprintf(stats,
    //         "CUDD: t%u n%u c%u u%.0f s%.0f h%.0f (%.0f%%) gc%d",
    //         manager() -> ReadSlots(),
    //         manager() -> ReadKeys(),
    //         manager() -> ReadCacheSlots(),
    //         manager() -> ReadCacheUsedSlots() * 100.0,
    //         manager() -> ReadCacheLookUps(),
    //         manager() -> ReadCacheHits(),
    //         (manager() -> ReadCacheHits() / manager() -> ReadCacheLookUps()) * 100.0,
    //         manager() -> ReadGarbageCollections());

    sprintf(stats_buffer, "CacBDDBMan: %d (CacBDDBooleRepr's)\n", num_reprs);
    return stats_buffer;
}

/*---------------------------------------------------------------------------*/

int
CacBDDBMan::size(BooleRepr * abr)
{
    // CacBDDBooleRepr * a = dcast(abr);
    //TODO
    // warning << "size not implemented\n";
}

/*---------------------------------------------------------------------------*/

void
CacBDDBMan::visualize(BooleRepr * abr)
{
    warning << "visualize not implemented in CacBDD\n";
}

/*---------------------------------------------------------------------------*/
