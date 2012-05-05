package com.xiaokan.test;

public class Solution {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Solution s = new Solution();
		int A[] = { 1, 0 };
		System.out.println(s.arrayJmp(A));

	}

	public int arrayJmp(int[] A) {
		// Check illegal A array
		if (A == null) {
			return 0;
		} else if (A.length == 0) {
			return 0;
		}
		// Array visited stores if this element is visited before: true,
		// otherwise: false
		boolean visited[] = new boolean[A.length];
		int count = 0, pos = 0;
		while (true) {
			count++;
			pos += A[pos];
			if (pos >= A.length) {
				// already jump out of array
				return count;
			} else if (visited[pos]) {
				// visited before, would never jump out of array
				return -1;
			}
			// mark it as visited
			visited[pos] = true;
		}
	}

}
