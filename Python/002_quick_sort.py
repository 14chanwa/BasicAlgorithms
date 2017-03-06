#
# Created by 14chanwa on 2017.02.28
#

# An implementation of the quick sort algorithm with 3 different pivot selection methods
# TODO: random pivot selection ?

def pivot_first(array, lb, ub):
    """
    Pivot selection method: returns the index of the first element of the array
    :param array:
    :param lb: lower bound of the array
    :param ub: upper bound of the array
    :return:
    """
    return lb


def pivot_last(array, lb, ub):
    """
    Pivot selection method: returns the index of the last element of the array
    :param array:
    :param lb: lower bound of the array
    :param ub: upper bound of the array
    :return:
    """
    return ub - 1


def pivot_median_of_three(array, lb, ub):
    """
    Pivot selection method: returns the index of the median element among the first, last and middle elements of the array
    :param array:
    :param lb: lower bound of the array
    :param ub: upper bound of the array
    :return:
    """
    median_index = (lb + ub) // 2
    if (ub - lb) % 2 == 0:
        median_index -= 1
    if array[lb] < array[median_index] < array[ub - 1] or array[ub - 1] < array[median_index] < array[lb]:
        return median_index
    elif array[lb] < array[ub - 1] < array[median_index] or array[median_index] < array[ub - 1] < array[lb]:
        return ub - 1
    else:
        return lb


def swap(array, id1, id2):
    """
    Swaps elements of indexes id1 and id2 in the given array
    :param array:
    :param id1:
    :param id2:
    """
    tmp = array[id1]
    array[id1] = array[id2]
    array[id2] = tmp


def quick_sort(array, lb, ub, pivot_method):
    """
    Recursively perform a quick sort on the part [lb, ub] of given array, using the provided pivot selection method.
    :param array:
    :param lb: lower bound of the array
    :param ub: upper bound of the array
    :param pivot_method:
    :return:
    """
    # Initial validity check
    if ub - lb < 2:
        return 0
    else:
        # Choose pivot
        pivot = pivot_method(array, lb, ub)

        # Swap first element
        swap(array, lb, pivot)

        # Rearrange elements
        # i: index position of last element <= pivot
        # j: index of current element
        i = lb
        for j in range(lb, ub):
            if array[j] < array[lb]:
                i += 1
                swap(array, i, j)
        swap(array, i, lb)

        # Count the number of comparisons and recursive calls if necessary
        total_sum = ub - lb - 1
        if i - lb > 1:
            total_sum += quick_sort(array, lb, i, pivot_method)
        if ub - (i + 1) > 1:
            total_sum += quick_sort(array, i + 1, ub, pivot_method)
        return total_sum
