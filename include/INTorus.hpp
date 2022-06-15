#pragma once

#include <array>
#include <cassert>
#include <cstdint>

namespace cuHEpp
{
template <typename T>
constexpr bool false_v = false;
constexpr uint64_t invpow2[31] = {
    9007061815787521ULL,
    13510592723681281ULL,
    15762358177628161ULL,
    16888240904601601ULL,
    17451182268088321ULL,
    17732652949831681ULL,
    17873388290703361ULL,
    17943755961139201ULL,
    17978939796357121ULL,
    17996531713966081ULL,
    18005327672770561ULL,
    18009725652172801ULL,
    18011924641873921ULL,
    18013024136724481ULL,
    18013573884149761ULL,
    18013848757862401ULL,
    18013986194718721ULL,
    18014054913146881ULL,
    18014089272360961ULL,
    18014106451968001ULL,
    18014115041771521ULL,
    18014119336673281ULL,
    18014121484124161ULL,
    18014122557849601ULL,
    18014123094712321ULL,
    18014123363143681ULL,
    18014123497359361ULL,
    18014123564467201ULL,
    18014123598021121ULL,
    18014123614798081ULL,
    18014123623186561ULL};
constexpr uint64_t P = (1ULL << 54) - (1ULL << 38) + 1;

// this class defines operations over integaer torus.
class INTorus
{
  public:
    uint64_t value;
    INTorus() { value = 0; }
    INTorus(uint64_t data, bool modulo = true)
    {
        if (modulo)
            value = data % P;
        else
            value = data;
    }

    // return this + b mod P.
    INTorus operator+(const INTorus &b) const
    {
        uint64_t tmp = static_cast<uint64_t>((static_cast<__uint128_t>(this->value) + b.value) % P);
        return INTorus(
            tmp, false);
    }

    INTorus &operator+=(const INTorus &b)
    {
        uint64_t tmp = static_cast<uint64_t>((static_cast<__uint128_t>(this->value) + b.value) % P);
        *this = INTorus(
            tmp,
            false);
        return *this;
    }

    // return this - b mod P.
    INTorus operator-(const INTorus &b) const
    {
        uint64_t tmp = static_cast<uint64_t>((static_cast<__uint128_t>(this->value) + P - b.value) % P);
        return INTorus(tmp,
                       false);
    }

    INTorus operator-=(const INTorus &b)
    {
        uint64_t tmp = static_cast<uint64_t>((static_cast<__uint128_t>(this->value) + P - b.value) % P);
        *this = INTorus(tmp,
                        false);
        return *this;
    }

    INTorus operator*(const INTorus &b) const
    {
        __uint128_t tmpa = static_cast<__uint128_t>(this->value) * b.value;
        uint64_t tmp = static_cast<uint64_t>(tmpa % P);
        return INTorus(tmp,
                       false);
    }

    INTorus operator*=(const INTorus &b)
    {
        const INTorus tmp = *this * b;
        *this = tmp;
        return *this;
    }

    INTorus operator<<(uint32_t l) const
    {
        if (l == 0) {
            return *this;
        }
        // t[0] = templ,t[1] = tempul, t[2] = tempuu
        else {
            uint64_t res = this->value;
            for (int i = l; i >= 63; i -= 63) {
                __uint128_t tmp = static_cast<__uint128_t>(res) << 63;
                res = static_cast<uint64_t>(tmp % P);
            }
            __uint128_t tmp = static_cast<__uint128_t>(res) << (l % 63);
            res = static_cast<uint64_t>(tmp % P);
            return INTorus(res);
        }
    }

    INTorus Pow(uint64_t e) const
    {
        INTorus res(1, false);
        for (uint64_t i = 0; i < e; i++)
            res *= *this;
        return res;
    }

    template <class Archive>
    void serialize(Archive &ar)
    {
        ar(value);
    }
};

// defined on [1,31]

inline INTorus InvPow2(uint8_t nbit)
{
    return INTorus(invpow2[nbit - 1]);
}
} // namespace cuHEpp
