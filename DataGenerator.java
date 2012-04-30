package com.xiaokan.generator;

import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.EOFException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Random;

public class DataGenerator {

	public static int numberOfLists = 100;

	public static int maxNumberOfPostingsInList = 10000;

	public static int minDGap = 100, maxDGap = 200;

	public static void main(String[] args) throws IOException {
		FileOutputStream fos = new FileOutputStream("d:/data.dat");
		Random rand = new Random();
		BufferedOutputStream fout = new BufferedOutputStream(fos);
		for (int i = 0; i < numberOfLists; i++) {
			int nOPIL = rand.nextInt(maxNumberOfPostingsInList);
			int buf[] = int2bytes(nOPIL);
			for (int k = 0; k < 4; k++)
				fout.write(buf[k]);
			int now = 1;
			for (int j = 1; j <= nOPIL; j++) {
				buf = int2bytes(now);
				now += rand.nextInt(maxDGap - minDGap) + maxDGap;
				for (int k = 0; k < 4; k++)
					fout.write(buf[k]);
			}
		}

	}

	public static int[] int2bytes(int v) {
		int b[] = new int[4];
		b[3] = (v >>> 24) & 0xFF;
		b[2] = (v >>> 16) & 0xFF;
		b[1] = (v >>> 8) & 0xFF;
		b[0] = (v >>> 0) & 0xFF;
		return b;
	}

	public static int bytes2int(byte[] bb) {
		int b[] = new int[4];
		for (int i = 0; i < 4; i++)
			b[i] = bb[i];
		return ((b[3] << 24) + (b[2] << 16) + (b[1] << 8) + (b[0] << 0));
	}

}
