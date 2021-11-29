/*
//@HEADER
// ************************************************************************
//
//                        Kokkos v. 2.0
//              Copyright (2019) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software. //
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#ifndef LINALG_INCLUDE_EXPERIMENTAL___P1673_BITS_CONJUGATE_IF_NEEDED_HPP_
#define LINALG_INCLUDE_EXPERIMENTAL___P1673_BITS_CONJUGATE_IF_NEEDED_HPP_

#include <complex>

namespace std {
namespace experimental {
inline namespace __p1673_version_0 {
namespace linalg {
namespace impl{

template<class T> struct is_complex : std::false_type{};

template<> struct is_complex<std::complex<float>> : std::true_type{};
template<> struct is_complex<std::complex<const float>> : std::true_type{};

template<> struct is_complex<std::complex<double>> : std::true_type{};
template<> struct is_complex<std::complex<const double>> : std::true_type{};

template<> struct is_complex<std::complex<long double>> : std::true_type{};
template<> struct is_complex<std::complex<const long double>> : std::true_type{};

template<class T> inline constexpr bool is_complex_v = is_complex<T>::value;

auto conj_if_needed = [](const auto value)
{
  using std::conj;
  // we need to use remove_cv because in some cases, e.g. gcc10,
  // decltype(value) carries a const qualification
  if constexpr (is_complex_v<std::remove_cv_t<decltype(value)>>) {
    return conj(value);
  } else {
    return value;
  }
};

} // end namespace impl
} // end namespace linalg
} // end inline namespace __p1673_version_0
} // end namespace experimental
} // end namespace std

#endif //LINALG_INCLUDE_EXPERIMENTAL___P1673_BITS_CONJUGATE_TRANSPOSED_HPP_
