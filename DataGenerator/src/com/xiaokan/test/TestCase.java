package com.xiaokan.test;

import java.io.FileOutputStream;
import java.io.IOException;


public class TestCase {

	public static void main(String args[]) throws IOException {

		
		FileOutputStream fos = new FileOutputStream("d:/d.dat");
		fos.write(0xABCDEF);
		fos.close();
	}
}
