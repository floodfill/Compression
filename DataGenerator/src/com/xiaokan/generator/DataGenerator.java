package com.xiaokan.generator;

import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Random;

public class DataGenerator {

	public static int numberOfLists = 100;

	public static int maxNumberOfPostingsInList = 10000;

	public static int minDGap = 100, maxDGap = 200;

	public static String outputFile = null;

	public static void showMsg() {
		System.out.println("Data Generator v1.0 by Kan Xiao");
		System.out.println("Usage: java DataGenerator out nOL mNOPIL minD maxD");
		System.out.println("    out: The Filename of Output Data");
		System.out.println("    nOL: Number of Lists");
		System.out.println("    mNOPIL: Max Number of Postings in List");
		System.out.println("    minD: min d-gap");
		System.out.println("    maxD: max d-gap");
	}

	public static void main(String[] args) {
		try {
			if (args.length < 5) {
				showMsg();
				return;
			}
			outputFile = args[0];
			numberOfLists = Integer.parseInt(args[1]);
			maxNumberOfPostingsInList = Integer.parseInt(args[2]);
			minDGap = Integer.parseInt(args[3]);
			maxDGap = Integer.parseInt(args[4]);
			int countOfNumbers = 0;
			System.out.format(
					"Number of Lists: %d, Max Number of Postings in List: %d, Min d-gap: %d, Max d-gap: %d\n",
					numberOfLists, maxNumberOfPostingsInList, minDGap, maxDGap);

			FileOutputStream fos = new FileOutputStream(outputFile);
			Random rand = new Random();
			BufferedOutputStream fout = new BufferedOutputStream(fos);
			for (int i = 0; i < numberOfLists; i++) {
				int nOPIL = rand.nextInt(maxNumberOfPostingsInList);
				int buf[] = int2bytes(nOPIL);
				for (int k = 0; k < 4; k++)
					fout.write(buf[k]);
				int now = 1;
				for (int j = 1; j <= nOPIL; j++) {
					countOfNumbers++;
					buf = int2bytes(now);
					now += rand.nextInt(maxDGap - minDGap) + maxDGap;
					for (int k = 0; k < 4; k++)
						fout.write(buf[k]);
				}
			}
			System.out.format("Number of Integers produced: %d\n", countOfNumbers);
			fos.close();
			fout.close();
		} catch (Exception e) {
			showMsg();
			e.printStackTrace();
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
