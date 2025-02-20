//@HEADER
// ************************************************************************
//
//                        Kokkos v. 4.0
//       Copyright (2022) National Technology & Engineering
//               Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Part of Kokkos, under the Apache License v2.0 with LLVM Exceptions.
// See https://kokkos.org/LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// ************************************************************************
//@HEADER

#ifndef LINALG_INCLUDE_EXPERIMENTAL___P1673_BITS_BLAS2_MATRIX_RANK_2_UPDATE_HPP_
#define LINALG_INCLUDE_EXPERIMENTAL___P1673_BITS_BLAS2_MATRIX_RANK_2_UPDATE_HPP_

namespace MDSPAN_IMPL_STANDARD_NAMESPACE {
namespace MDSPAN_IMPL_PROPOSED_NAMESPACE {
inline namespace __p1673_version_0 {
namespace linalg {

namespace {

template <class Exec, class x_t, class y_t, class A_t, class Tr_t, class = void>
struct is_custom_symmetric_matrix_rank_2_update_avail : std::false_type {};

template <class Exec, class x_t, class y_t, class A_t, class Tr_t>
struct is_custom_symmetric_matrix_rank_2_update_avail<
  Exec, x_t, y_t, A_t, Tr_t,
  std::enable_if_t<
    std::is_void_v<
      decltype(symmetric_matrix_rank_2_update
	       (std::declval<Exec>(),
		std::declval<x_t>(),
		std::declval<y_t>(),
		std::declval<A_t>(),
		std::declval<Tr_t>()
		)
	       )
      >
    && ! impl::is_inline_exec_v<Exec>
    >
  >
  : std::true_type{};

template <class Exec, class x_t, class y_t, class A_t, class Tr_t, class = void>
struct is_custom_hermitian_matrix_rank_2_update_avail : std::false_type {};

template <class Exec, class x_t, class y_t, class A_t, class Tr_t>
struct is_custom_hermitian_matrix_rank_2_update_avail<
  Exec, x_t, y_t, A_t, Tr_t,
  std::enable_if_t<
    std::is_void_v<
      decltype(hermitian_matrix_rank_2_update
	       (std::declval<Exec>(),
		std::declval<x_t>(),
		std::declval<y_t>(),
		std::declval<A_t>(),
		std::declval<Tr_t>()
		)
	       )
      >
    && ! impl::is_inline_exec_v<Exec>
    >
  >
  : std::true_type{};

} // end anonymous namespace

// Rank-2 update of a symmetric matrix

template<class ElementType_x,
         class SizeType_x, ::std::size_t ext_x,
         class Layout_x,
         class Accessor_x,
         class ElementType_y,
         class SizeType_y, ::std::size_t ext_y,
         class Layout_y,
         class Accessor_y,
         class ElementType_A,
         class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
         class Layout_A,
         class Accessor_A,
         class Triangle>
void symmetric_matrix_rank_2_update(
  impl::inline_exec_t&& /* exec */,
  mdspan<ElementType_x, extents<SizeType_x, ext_x>, Layout_x, Accessor_x> x,
  mdspan<ElementType_y, extents<SizeType_y, ext_y>, Layout_y, Accessor_y> y,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  Triangle /* t */)
{
  using size_type = std::common_type_t<SizeType_x, SizeType_y, SizeType_A>;
  constexpr bool lower_tri =
    std::is_same_v<Triangle, lower_triangle_t>;
  for (size_type j = 0; j < A.extent(1); ++j) {
    const size_type i_lower = lower_tri ? j : size_type(0);
    const size_type i_upper = lower_tri ? A.extent(0) : j+1;

    for (size_type i = i_lower; i < i_upper; ++i) {
      A(i,j) += x(i) * y(j) + y(i) * x(j);
    }
  }
}

template<class ExecutionPolicy,
         class ElementType_x,
         class SizeType_x, ::std::size_t ext_x,
         class Layout_x,
         class Accessor_x,
         class ElementType_y,
         class SizeType_y, ::std::size_t ext_y,
         class Layout_y,
         class Accessor_y,
         class ElementType_A,
         class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
         class Layout_A,
         class Accessor_A,
         class Triangle>
void symmetric_matrix_rank_2_update(
  ExecutionPolicy&& exec,
  mdspan<ElementType_x, extents<SizeType_x, ext_x>, Layout_x, Accessor_x> x,
  mdspan<ElementType_y, extents<SizeType_y, ext_y>, Layout_y, Accessor_y> y,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  Triangle t)
{
  constexpr bool use_custom = is_custom_symmetric_matrix_rank_2_update_avail<
    decltype(impl::map_execpolicy_with_check(exec)), decltype(x), decltype(y), decltype(A), Triangle
    >::value;

  if constexpr (use_custom) {
    symmetric_matrix_rank_2_update(impl::map_execpolicy_with_check(exec), x, y, A, t);
  }
  else {
    symmetric_matrix_rank_2_update(impl::inline_exec_t{}, x, y, A, t);
  }
}

template<class ElementType_x,
         class SizeType_x, ::std::size_t ext_x,
         class Layout_x,
         class Accessor_x,
         class ElementType_y,
         class SizeType_y, ::std::size_t ext_y,
         class Layout_y,
         class Accessor_y,
         class ElementType_A,
         class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
         class Layout_A,
         class Accessor_A,
         class Triangle>
void symmetric_matrix_rank_2_update(
  mdspan<ElementType_x, extents<SizeType_x, ext_x>, Layout_x, Accessor_x> x,
  mdspan<ElementType_y, extents<SizeType_y, ext_y>, Layout_y, Accessor_y> y,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  Triangle t)
{
  symmetric_matrix_rank_2_update(impl::default_exec_t{}, x, y, A, t);
}


// Rank-2 update of a Hermitian matrix

template<class ElementType_x,
         class SizeType_x, ::std::size_t ext_x,
         class Layout_x,
         class Accessor_x,
         class ElementType_y,
         class SizeType_y, ::std::size_t ext_y,
         class Layout_y,
         class Accessor_y,
         class ElementType_A,
         class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
         class Layout_A,
         class Accessor_A,
         class Triangle>
void hermitian_matrix_rank_2_update(
  impl::inline_exec_t&& /* exec */,
  mdspan<ElementType_x, extents<SizeType_x, ext_x>, Layout_x, Accessor_x> x,
  mdspan<ElementType_y, extents<SizeType_y, ext_y>, Layout_y, Accessor_y> y,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  Triangle /* t */)
{
  using size_type = std::common_type_t<SizeType_x, SizeType_y, SizeType_A>;

  constexpr bool lower_tri =
    std::is_same_v<Triangle, lower_triangle_t>;
  for (size_type j = 0; j < A.extent(1); ++j) {
    const size_type i_lower = lower_tri ? j : size_type(0);
    const size_type i_upper = lower_tri ? A.extent(0) : j+1;

    A(j,j) = impl::real_if_needed(A(j,j));
    for (size_type i = i_lower; i < i_upper; ++i) {
      A(i,j) += x(i) * impl::conj_if_needed(y(j)) + y(i) * impl::conj_if_needed(x(j));
    }
  }
}

template<class ExecutionPolicy,
         class ElementType_x,
         class SizeType_x, ::std::size_t ext_x,
         class Layout_x,
         class Accessor_x,
         class ElementType_y,
         class SizeType_y, ::std::size_t ext_y,
         class Layout_y,
         class Accessor_y,
         class ElementType_A,
         class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
         class Layout_A,
         class Accessor_A,
         class Triangle>
void hermitian_matrix_rank_2_update(
  ExecutionPolicy&& exec,
  mdspan<ElementType_x, extents<SizeType_x, ext_x>, Layout_x, Accessor_x> x,
  mdspan<ElementType_y, extents<SizeType_y, ext_y>, Layout_y, Accessor_y> y,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  Triangle t)
{
  constexpr bool use_custom = is_custom_hermitian_matrix_rank_2_update_avail<
    decltype(impl::map_execpolicy_with_check(exec)), decltype(x), decltype(y), decltype(A), Triangle
    >::value;

  if constexpr (use_custom) {
    hermitian_matrix_rank_2_update(impl::map_execpolicy_with_check(exec), x, y, A, t);
  }
  else {
    hermitian_matrix_rank_2_update(impl::inline_exec_t{}, x, y, A, t);
  }
}

template<class ElementType_x,
         class SizeType_x, ::std::size_t ext_x,
         class Layout_x,
         class Accessor_x,
         class ElementType_y,
         class SizeType_y, ::std::size_t ext_y,
         class Layout_y,
         class Accessor_y,
         class ElementType_A,
         class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
         class Layout_A,
         class Accessor_A,
         class Triangle>
void hermitian_matrix_rank_2_update(
  mdspan<ElementType_x, extents<SizeType_x, ext_x>, Layout_x, Accessor_x> x,
  mdspan<ElementType_y, extents<SizeType_y, ext_y>, Layout_y, Accessor_y> y,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  Triangle t)
{
  hermitian_matrix_rank_2_update(impl::default_exec_t{}, x, y, A, t);
}

} // end namespace linalg
} // end inline namespace __p1673_version_0
} // end namespace MDSPAN_IMPL_PROPOSED_NAMESPACE
} // end namespace MDSPAN_IMPL_STANDARD_NAMESPACE

#endif //LINALG_INCLUDE_EXPERIMENTAL___P1673_BITS_BLAS2_MATRIX_RANK_2_UPDATE_HPP_
