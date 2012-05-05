package com.xiaokan.algo;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class DiaosiEncoder {

	public static void main(String[] args) throws IOException {
		args = new String[10];
		if (args.length < 2) {
			System.out.println("Diaosi Encoder ver1.0 by Kan XIAO");
			System.out.println("Usage:");
			System.out.println("java DiaosiEncoder infile outfilePrefix");
			System.out.println("After valid compression it will produce outfilePrefix.dat & outfilePrefix.inf");
		}

		args[0] = "d:/d.dat";
		args[1] = "d:/com";
		FileInputStream fis = new FileInputStream(args[0]);
		BufferedInputStream fin = new BufferedInputStream(fis);

		ArrayList<Integer> sel = new ArrayList<Integer>();

		FileOutputStream fos = new FileOutputStream(args[1] + ".dat");
		BufferedOutputStream fout = new BufferedOutputStream(fos);
		int remain = 0;
		while (fin.available() > 0) {
			byte[] buf = new byte[4];
			fin.read(buf);
			int N = bytes2int(buf);
			sel.add(N);
			int last = -1;
			for (int i = 0; i < N; i++) {
				fin.read(buf);
				int num = bytes2int(buf);
				assert last == -1 || last < num;
				int temp = num;
				if (last != -1)
					num -= last;
				last = temp;
				int bits = getNumberOfBits(num);
				sel.add(bits);

				// remains bytes from the last int
				if (remain != 0) {
					int rBits = getNumberOfBits(remain);
					if (bits > 8 - rBits) {
						temp = num % (2 << (8 - rBits));
						temp <<= rBits;
						temp += remain;
						fout.write(temp);
						remain = 0;
						num >>>= 8 - rBits;
						bits -= rBits;
					} else {
						num <<= rBits;
						num += remain;
						remain = 0;
						bits = 0;
						bits += rBits;
					}
				}

				while (bits >= 8) {
					fout.write(num);
					num >>>= 8;
					bits -= 8;
				}

				if (bits > 0)
					remain = num;
				else
					remain = 0;

			}

		}
		if (remain != 0)
			fout.write(remain);
		fout.flush();
		fout.close();
		fos.close();
		fin.close();
		fis.close();

		// write out selectors

		S16Enc s = new S16Enc();
		for (int i = 0; i < sel.size(); i++)
			System.out.format("%d ", sel.get(i));
		byte selEncoded[] = s.encode(sel);
		fos = new FileOutputStream(args[1] + ".inf");
		fout = new BufferedOutputStream(fos);
		fout.write(selEncoded);
		fout.flush();
		fout.close();

	}

	private static int getNumberOfBits(int n) {
		int count = 0;
		while (n != 0) {
			n >>>= 1;
			count++;
		}
		return count;
	}

	private static int bytes2int(byte[] bb) {
		int b[] = new int[4];
		for (int i = 0; i < 4; i++) {
			b[i] = bb[i];
			if (b[i] < 0) {
				b[i] &= 0xff;
				b[i] |= 0x80;
			}
		}
		return ((b[3] << 24) + (b[2] << 16) + (b[1] << 8) + (b[0] << 0));
	}

}

class S16Enc {

	public static byte[] intToByteArray(final int x) {
		final byte[] bytes = new byte[4];
		bytes[3] = (byte) (x & 0x000000ff);
		bytes[2] = (byte) ((x & 0x0000ff00) >>> 8);
		bytes[1] = (byte) ((x & 0x00ff0000) >>> 16);
		bytes[0] = (byte) ((x & 0xff000000) >>> 24);
		return bytes;
	}

	public int byteArrayToInt(final byte[] b) {
		int value = 0;
		for (int i = 0; i < 4; i++) {
			value += (b[i] & 0x000000FF) << (4 - 1 - i) * 8;
		}
		return value;
	}

	public int[] decode(final byte[] codes) throws IOException {
		// An array list of integers
		int buff[] = new int[1000000];
		int count = 0;
		final ByteArrayInputStream input = new ByteArrayInputStream(codes);
		final byte[] reusableByteArray = new byte[4];
		int word = 0;
		while (input.available() > 0) {
			input.read(reusableByteArray);
			word = byteArrayToInt(reusableByteArray);
			int[] d = this.decode(word);
			for (int i : d)
				buff[count++] = i;
		}
		return Arrays.copyOf(buff, count);
	}

	public byte[] encode(final List<Integer> values) throws IOException {
		final ByteArrayOutputStream out = new ByteArrayOutputStream();

		int i = 0;

		// While we didn't encode all the values, continue
		while (i < values.size()) {
			int word = 0;

			// Start iteration over selectors
			for (int k = 0; k < 16; k++) {
				int j = 0, ncodes = 0;
				// Encode selector
				word = k << 28;
				// How many codes we can fit ?
				ncodes = (cnum[k] < values.size() - i) ? cnum[k] : values.size() - i;

				// Start iteration over values
				// if a value is > to the allowed code size, break loop and try
				// next selector
				for (int shift = 0; (j < ncodes) && values.get(i + j) < (1 << cbits[k][j]); j++) {
					word += values.get(i + j) << shift;
					// Increment shift with code width
					shift += cbits[k][j];
				}

				// if all codes have been fitted, convert the word and return a
				// byte array
				if (j == ncodes) {
					i = i + ncodes;
					// Append word
					out.write(intToByteArray(word));
					// Break the for loop
					break;
				}
			}
		}

		return out.toByteArray();
	}

	protected int[] decode(final int word) {
		int[] values;
		int i = 0;
		// Unsigned right shift
		final int selector = word >>> 28;

		switch (selector) {
		case 0:
			values = new int[28];

			values[i] = word & 1;
			i++;
			values[i] = (word >> 1) & 1;
			i++;
			values[i] = (word >> 2) & 1;
			i++;
			values[i] = (word >> 3) & 1;
			i++;
			values[i] = (word >> 4) & 1;
			i++;
			values[i] = (word >> 5) & 1;
			i++;
			values[i] = (word >> 6) & 1;
			i++;
			values[i] = (word >> 7) & 1;
			i++;
			values[i] = (word >> 8) & 1;
			i++;
			values[i] = (word >> 9) & 1;
			i++;
			values[i] = (word >> 10) & 1;
			i++;
			values[i] = (word >> 11) & 1;
			i++;
			values[i] = (word >> 12) & 1;
			i++;
			values[i] = (word >> 13) & 1;
			i++;
			values[i] = (word >> 14) & 1;
			i++;
			values[i] = (word >> 15) & 1;
			i++;
			values[i] = (word >> 16) & 1;
			i++;
			values[i] = (word >> 17) & 1;
			i++;
			values[i] = (word >> 18) & 1;
			i++;
			values[i] = (word >> 19) & 1;
			i++;
			values[i] = (word >> 20) & 1;
			i++;
			values[i] = (word >> 21) & 1;
			i++;
			values[i] = (word >> 22) & 1;
			i++;
			values[i] = (word >> 23) & 1;
			i++;
			values[i] = (word >> 24) & 1;
			i++;
			values[i] = (word >> 25) & 1;
			i++;
			values[i] = (word >> 26) & 1;
			i++;
			values[i] = (word >> 27) & 1;
			i++;
			return values;

		case 1:
			values = new int[21];

			values[i] = (word) & 3;
			i++;
			values[i] = (word >> 2) & 3;
			i++;
			values[i] = (word >> 4) & 3;
			i++;
			values[i] = (word >> 6) & 3;
			i++;
			values[i] = (word >> 8) & 3;
			i++;
			values[i] = (word >> 10) & 3;
			i++;
			values[i] = (word >> 12) & 3;
			i++;
			values[i] = (word >> 14) & 1;
			i++;
			values[i] = (word >> 15) & 1;
			i++;
			values[i] = (word >> 16) & 1;
			i++;
			values[i] = (word >> 17) & 1;
			i++;
			values[i] = (word >> 18) & 1;
			i++;
			values[i] = (word >> 19) & 1;
			i++;
			values[i] = (word >> 20) & 1;
			i++;
			values[i] = (word >> 21) & 1;
			i++;
			values[i] = (word >> 22) & 1;
			i++;
			values[i] = (word >> 23) & 1;
			i++;
			values[i] = (word >> 24) & 1;
			i++;
			values[i] = (word >> 25) & 1;
			i++;
			values[i] = (word >> 26) & 1;
			i++;
			values[i] = (word >> 27) & 1;
			i++;
			return values;

		case 2:
			values = new int[21];

			values[i] = (word) & 1;
			i++;
			values[i] = (word >> 1) & 1;
			i++;
			values[i] = (word >> 2) & 1;
			i++;
			values[i] = (word >> 3) & 1;
			i++;
			values[i] = (word >> 4) & 1;
			i++;
			values[i] = (word >> 5) & 1;
			i++;
			values[i] = (word >> 6) & 1;
			i++;
			values[i] = (word >> 7) & 3;
			i++;
			values[i] = (word >> 9) & 3;
			i++;
			values[i] = (word >> 11) & 3;
			i++;
			values[i] = (word >> 13) & 3;
			i++;
			values[i] = (word >> 15) & 3;
			i++;
			values[i] = (word >> 17) & 3;
			i++;
			values[i] = (word >> 19) & 3;
			i++;
			values[i] = (word >> 21) & 1;
			i++;
			values[i] = (word >> 22) & 1;
			i++;
			values[i] = (word >> 23) & 1;
			i++;
			values[i] = (word >> 24) & 1;
			i++;
			values[i] = (word >> 25) & 1;
			i++;
			values[i] = (word >> 26) & 1;
			i++;
			values[i] = (word >> 27) & 1;
			i++;
			return values;

		case 3:
			values = new int[21];

			values[i] = (word) & 1;
			i++;
			values[i] = (word >> 1) & 1;
			i++;
			values[i] = (word >> 2) & 1;
			i++;
			values[i] = (word >> 3) & 1;
			i++;
			values[i] = (word >> 4) & 1;
			i++;
			values[i] = (word >> 5) & 1;
			i++;
			values[i] = (word >> 6) & 1;
			i++;
			values[i] = (word >> 7) & 1;
			i++;
			values[i] = (word >> 8) & 1;
			i++;
			values[i] = (word >> 9) & 1;
			i++;
			values[i] = (word >> 10) & 1;
			i++;
			values[i] = (word >> 11) & 1;
			i++;
			values[i] = (word >> 12) & 1;
			i++;
			values[i] = (word >> 13) & 1;
			i++;
			values[i] = (word >> 14) & 3;
			i++;
			values[i] = (word >> 16) & 3;
			i++;
			values[i] = (word >> 18) & 3;
			i++;
			values[i] = (word >> 20) & 3;
			i++;
			values[i] = (word >> 22) & 3;
			i++;
			values[i] = (word >> 24) & 3;
			i++;
			values[i] = (word >> 26) & 3;
			i++;
			return values;

		case 4:
			values = new int[14];

			values[i] = (word) & 3;
			i++;
			values[i] = (word >> 2) & 3;
			i++;
			values[i] = (word >> 4) & 3;
			i++;
			values[i] = (word >> 6) & 3;
			i++;
			values[i] = (word >> 8) & 3;
			i++;
			values[i] = (word >> 10) & 3;
			i++;
			values[i] = (word >> 12) & 3;
			i++;
			values[i] = (word >> 14) & 3;
			i++;
			values[i] = (word >> 16) & 3;
			i++;
			values[i] = (word >> 18) & 3;
			i++;
			values[i] = (word >> 20) & 3;
			i++;
			values[i] = (word >> 22) & 3;
			i++;
			values[i] = (word >> 24) & 3;
			i++;
			values[i] = (word >> 26) & 3;
			i++;
			return values;

		case 5:
			values = new int[9];

			values[i] = (word) & 15;
			i++;
			values[i] = (word >> 4) & 7;
			i++;
			values[i] = (word >> 7) & 7;
			i++;
			values[i] = (word >> 10) & 7;
			i++;
			values[i] = (word >> 13) & 7;
			i++;
			values[i] = (word >> 16) & 7;
			i++;
			values[i] = (word >> 19) & 7;
			i++;
			values[i] = (word >> 22) & 7;
			i++;
			values[i] = (word >> 25) & 7;
			i++;
			return values;

		case 6:
			values = new int[8];

			values[i] = (word) & 7;
			i++;
			values[i] = (word >> 3) & 15;
			i++;
			values[i] = (word >> 7) & 15;
			i++;
			values[i] = (word >> 11) & 15;
			i++;
			values[i] = (word >> 15) & 15;
			i++;
			values[i] = (word >> 19) & 7;
			i++;
			values[i] = (word >> 22) & 7;
			i++;
			values[i] = (word >> 25) & 7;
			i++;
			return values;

		case 7:
			values = new int[7];

			values[i] = (word) & 15;
			i++;
			values[i] = (word >> 4) & 15;
			i++;
			values[i] = (word >> 8) & 15;
			i++;
			values[i] = (word >> 12) & 15;
			i++;
			values[i] = (word >> 16) & 15;
			i++;
			values[i] = (word >> 20) & 15;
			i++;
			values[i] = (word >> 24) & 15;
			i++;
			return values;

		case 8:
			values = new int[6];

			values[i] = (word) & 31;
			i++;
			values[i] = (word >> 5) & 31;
			i++;
			values[i] = (word >> 10) & 31;
			i++;
			values[i] = (word >> 15) & 31;
			i++;
			values[i] = (word >> 20) & 15;
			i++;
			values[i] = (word >> 24) & 15;
			i++;
			return values;

		case 9:
			values = new int[6];

			values[i] = (word) & 15;
			i++;
			values[i] = (word >> 4) & 15;
			i++;
			values[i] = (word >> 8) & 31;
			i++;
			values[i] = (word >> 13) & 31;
			i++;
			values[i] = (word >> 18) & 31;
			i++;
			values[i] = (word >> 23) & 31;
			i++;
			return values;

		case 10:
			values = new int[5];

			values[i] = (word) & 63;
			i++;
			values[i] = (word >> 6) & 63;
			i++;
			values[i] = (word >> 12) & 63;
			i++;
			values[i] = (word >> 18) & 31;
			i++;
			values[i] = (word >> 23) & 31;
			i++;
			return values;

		case 11:
			values = new int[5];

			values[i] = (word) & 31;
			i++;
			values[i] = (word >> 5) & 31;
			i++;
			values[i] = (word >> 10) & 63;
			i++;
			values[i] = (word >> 16) & 63;
			i++;
			values[i] = (word >> 22) & 63;
			i++;
			return values;

		case 12:
			values = new int[4];

			values[i] = (word) & 127;
			i++;
			values[i] = (word >> 7) & 127;
			i++;
			values[i] = (word >> 14) & 127;
			i++;
			values[i] = (word >> 21) & 127;
			i++;
			return values;

		case 13:
			values = new int[3];

			values[i] = (word) & 1023;
			i++;
			values[i] = (word >> 10) & 511;
			i++;
			values[i] = (word >> 19) & 511;
			i++;
			return values;

		case 14:
			values = new int[2];

			values[i] = (word) & 16383;
			i++;
			values[i] = (word >> 14) & 16383;
			i++;
			return values;

		case 15:
			values = new int[1];

			values[i] = (word) & ((1 << 28) - 1);
			i++;
			return values;

		default:
			throw new RuntimeException("Unknown selector: " + selector);
		}

	}

	protected static int cbits[][] = {
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0 },
			{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 4, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 3, 4, 4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 5, 5, 5, 5, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 4, 4, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 6, 6, 6, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 5, 5, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 10, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 14, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

	protected static int cnum[] = { 28, 21, 21, 21, 14, 9, 8, 7, 6, 6, 5, 5, 4, 3, 2, 1 };

}
