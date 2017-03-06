#
# Created by 14chanwa on 2017.02.28
#

# Given an int array, merge sort (recursively) and count inversions

def count_inversions_and_merge_sort(array):
    """
    Count inversions and merge sort in given array
    :param array: 
    :return: sorted array, number of inversions
    """
    if len(array) < 2:
        return array, 0
    else:
        # Split and recursively count inversions and sort
        pivot = len(array)//2
        array1, cinv1 = count_inversions_and_merge_sort(array[:pivot])
        array2, cinv2 = count_inversions_and_merge_sort(array[pivot:])

        cinv = cinv1 + cinv2

        # Merge
        i = 0
        j = 0
        final_array = []
        while len(final_array) < len(array):
            if i < len(array1) and j < len(array2):
                if array2[j] < array1[i]:
                    cinv += len(array1) - i
                    final_array.append(array2[j])
                    j += 1
                else:
                    final_array.append(array1[i])
                    i += 1
            elif j < len(array2):
                final_array.append(array2[j])
                j += 1
            elif i < len(array1):
                final_array.append(array1[i])
                i += 1
        return final_array, cinv
