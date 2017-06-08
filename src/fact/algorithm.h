#pragma once

namespace FactUtilEmbedded { namespace std {

/**
 * "When copying overlapping ranges, std::copy is appropriate when copying to the left
 * (beginning of the destination range is outside the source range)"
 * - cppreference.com
 *
 * @tparam InputIt
 * @tparam OutputIt
 * @param first
 * @param last
 * @param d_first
 * @return
 */
template< class InputIt, class OutputIt >
OutputIt copy( InputIt first, InputIt last, OutputIt d_first )
{
    while (first != last) {
        *d_first++ = *first++;
    }
    return d_first;
}


/**
 * "std::copy_backward is appropriate when copying to the right (end of the destination range is outside the source range)."
 * -- cppreference.com
 * @tparam BidirIt1
 * @tparam BidirIt2
 * @param first
 * @param last
 * @param d_last
 * @return
 */
template< class BidirIt1, class BidirIt2 >
BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
{
    while (first != last) {
        *(--d_last) = *(--last);
    }
    return d_last;
}

}}