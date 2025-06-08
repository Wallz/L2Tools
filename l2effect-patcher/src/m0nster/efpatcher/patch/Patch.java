package m0nster.efpatcher.patch;

import java.util.List;

import acmi.l2.clientmod.io.UnrealPackageFile;
import acmi.l2.clientmod.unreal.classloader.L2Property;
import acmi.l2.clientmod.unreal.core.Object;

/**
 * @author PointerRage
 *
 */
public abstract class Patch {
	private List<Object> objects;
	
	public abstract void patch(UnrealPackageFile upf, L2Property property, String value);
	
	protected List<Object> getObjects() {
		return objects;
	}
	
	public void setObjects(List<Object> objects) {
		this.objects = objects;
	}
	
	
	protected void printf(String line, java.lang.Object...args) {
		System.out.printf(line, args);
	}
	
	protected void out() {
		System.out.println();
	}
	
	protected void out(String s) {
		System.out.println(s);
	}
	
	protected String printData(byte[] data, int len) {
		StringBuffer result = new StringBuffer();

		int counter = 0;

		for (int i = 0; i < len; i++) {
			if (counter % 16 == 0)
				result.append(fillHex(i, 4) + ": ");

			result.append(fillHex(data[i] & 0xff, 2) + " ");
			counter++;
			if (counter == 16) {
				result.append("\n");
				counter = 0;
			}
		}

		return result.toString();
	}

	protected String fillHex(int data, int digits) {
		String number = Integer.toHexString(data);

		for (int i = number.length(); i < digits; i++)
			number = "0" + number;

		return number;
	}
	
	protected String printData(byte[] raw) {
		return printData(raw, raw.length);
	}
}
