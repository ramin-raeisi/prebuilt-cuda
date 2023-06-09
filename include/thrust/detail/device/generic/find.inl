/*
 *  Copyright 2008-2011 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <thrust/detail/device/reduce.h>

#include <thrust/tuple.h>
#include <thrust/extrema.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/iterator/transform_iterator.h>
#include <thrust/iterator/zip_iterator.h>

// Contributed by Erich Elsen

namespace thrust
{
namespace detail
{
namespace device
{
namespace generic
{

template <typename TupleType>
struct find_if_functor
{
    __host__ __device__
    TupleType operator()(const TupleType& lhs, const TupleType& rhs) const
    {
        // select the smallest index among true results
        if (thrust::get<0>(lhs) && thrust::get<0>(rhs))
            return TupleType(true, (thrust::min)(thrust::get<1>(lhs), thrust::get<1>(rhs)));
        else if (thrust::get<0>(lhs))
            return lhs;
        else
            return rhs;
    }
};
    

template <typename InputIterator, typename Predicate>
InputIterator find_if(InputIterator first,
                      InputIterator last,
                      Predicate pred)
{
    typedef typename thrust::iterator_traits<InputIterator>::difference_type difference_type;
    typedef typename thrust::tuple<bool,difference_type> result_type;
   
    // empty sequence
    if (first == last)
        return last;

    const difference_type n = thrust::distance(first, last);

    // this implementation breaks up the sequence into separate intervals
    // in an attempt to early-out as soon as a value is found

    // TODO incorporate sizeof(InputType) into interval_threshold and round to multiple of 32
    const difference_type interval_threshold = 1 << 20;
    const difference_type interval_size = (std::min)(interval_threshold, n);

    // XXX WAR nvcc 3.2 + linux codegen problems with this functionally equivalent predicate
    typedef thrust::detail::unary_negate<thrust::detail::unary_negate<Predicate> > NotNotPred;

    typedef thrust::transform_iterator<NotNotPred, InputIterator, bool> XfrmIterator;
    typedef thrust::tuple<XfrmIterator, thrust::counting_iterator<difference_type> > IteratorTuple;
    typedef thrust::zip_iterator<IteratorTuple> ZipIterator;

    NotNotPred not_not_pred = thrust::detail::not1(thrust::detail::not1(pred));

    IteratorTuple iter_tuple = thrust::make_tuple(XfrmIterator(first, not_not_pred),
                                                  thrust::counting_iterator<difference_type>(0));

    ZipIterator begin = thrust::make_zip_iterator(iter_tuple);
    ZipIterator end   = begin + n;

    for(ZipIterator interval_begin = begin; interval_begin < end; interval_begin += interval_size)
    {
        ZipIterator interval_end = interval_begin + interval_size;
        if(end < interval_end)
        {
          interval_end = end;
        } // end if

        result_type result = thrust::detail::device::reduce(interval_begin, interval_end,
                                                            result_type(false,interval_end - begin),
                                                            find_if_functor<result_type>());

        // see if we found something
        if (thrust::get<0>(result))
        {
            return first + thrust::get<1>(result);
        }
    }

    //nothing was found if we reach here...
    return first + n;
}

} // end namespace generic
} // end namespace device
} // end namespace detail
} // end namespace thrust

