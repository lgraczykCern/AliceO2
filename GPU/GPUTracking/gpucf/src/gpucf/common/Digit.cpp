#include "Digit.h"

#include <gpucf/common/RowInfo.h>

#include <ostream>
#include <sstream>


using namespace gpucf;


static_assert(sizeof(Digit) == sizeof(PackedDigit));
static_assert(sizeof(PackedDigit) == PACKED_DIGIT_SIZE);


Digit::Digit()
    : Digit(0.f, 0, 0, 0)
{
}

Digit::Digit(float _charge, int _row, int _pad, int _time)
{
    charge = _charge;
    row = _row;
    pad = _pad;
    time = _time;
}

Object Digit::serialize() const
{
    Object obj("Digit");

    SET_FIELD(obj, row);
    SET_FIELD(obj, pad);
    SET_FIELD(obj, time);
    SET_FIELD(obj, charge);

    return obj;
}

void Digit::deserialize(const Object &o)
{
    /* GET_INT(o, cru); */
    GET_INT(o, row);
    GET_INT(o, pad);
    GET_INT(o, time);
    GET_FLOAT(o, charge);
}

int Digit::localRow() const
{
    return RowInfo::instance().globalToLocal(row);
}

int Digit::cru() const
{
    return RowInfo::instance().globalRowToCru(row);
}

bool Digit::operator==(const Digit &other) const
{
    bool eq = true;
    eq &= (row == other.row);
    eq &= (pad == other.pad);
    eq &= (time == other.time);
    eq &= (charge == other.charge);

    return eq;
}

std::ostream &gpucf::operator<<(std::ostream &os, const Digit &d) 
{
    return os << d.serialize();
}

// vim: set ts=4 sw=4 sts=4 expandtab:
