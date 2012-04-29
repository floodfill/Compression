import java.io.BufferedInputStream;
import java.io.FileInputStream;

public class EnsureIncreasingOrder {

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception {
		FileInputStream fis = new FileInputStream("E:/inverted_index/INVERTED_INDEX_FILE.DAT");
		BufferedInputStream fin = new BufferedInputStream(fis);
		byte buf[] = new byte[4];
		while (fin.available() != 0) {
			fin.read(buf);
			int N = bytes2int(buf);
			//System.out.println(N);
			int last = -1;
			for (int i = 0; i < N; i++) {
				fin.read(buf);
				int M = bytes2int(buf);
				//System.out.print(M + " ");
				assert M > last;
				last = M;

			}
			//System.out.println();
		}

	}

	public static int bytes2int(byte[] bb) {
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
