template <class BidirectionalIterator>
bool next_partial_permutation(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
    reverse(middle, last);
    return next_permutation(first , last);
}