package drone.util;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.GraphicsConfiguration;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.HeadlessException;
import java.awt.Image;
import java.awt.Transparency;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.awt.image.PixelGrabber;

import javax.swing.ImageIcon;

public class ImageConvert {

	/**
	 * Converts an AWT Image into a grayscale BufferedImage.
	 * 
	 * @param image the image to convert
	 * @return a BufferedImage
	 */
	public static BufferedImage toGrayBufferedImage(Image image) {
		if (image instanceof BufferedImage && ((BufferedImage)image).getType() == BufferedImage.TYPE_BYTE_GRAY)
			return (BufferedImage)image;

		// This code ensures that all the pixels in the image are loaded
		image = new ImageIcon(image).getImage();
		// Create a buffered image with a format that's compatible with the screen
		BufferedImage bimage = new BufferedImage(image.getWidth(null), image.getHeight(null), BufferedImage.TYPE_BYTE_GRAY);

		// Copy image to buffered image
		Graphics g = bimage.createGraphics();

		// Paint the image onto the buffered image
		g.drawImage(image, 0, 0, null);
		g.dispose();

		return bimage;
	}


	/**
	 * Converts an AWT Image into a BufferedImage.
	 * 
	 * @param image the image to convert
	 * @return a BufferedImage with the contents of the image
	 */
	public static BufferedImage toBufferedImage(Image image) {
		if (image instanceof BufferedImage) {
			return (BufferedImage)image;
		}

		// This code ensures that all the pixels in the image are loaded
		image = new ImageIcon(image).getImage();

		// Determine if the image has transparent pixels; for this method's
		// implementation, see e661 Determining If an Image Has Transparent Pixels
		boolean hasAlpha = hasAlpha(image);

		// Create a buffered image with a format that's compatible with the screen
		BufferedImage bimage = null;
		GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
		try {
			// Determine the type of transparency of the new buffered image
			int transparency = hasAlpha ? Transparency.BITMASK : Transparency.OPAQUE;

			// Create the buffered image
			GraphicsDevice gs = ge.getDefaultScreenDevice();
			GraphicsConfiguration gc = gs.getDefaultConfiguration();
			bimage = gc.createCompatibleImage(
					image.getWidth(null), image.getHeight(null), transparency);
		} catch (HeadlessException e) {
			// The system does not have a screen
		}

		if (bimage == null) {
			// Create a buffered image using the default color model
			int type = hasAlpha ? BufferedImage.TYPE_INT_ARGB : BufferedImage.TYPE_INT_RGB;
			bimage = new BufferedImage(image.getWidth(null), image.getHeight(null), type);
		}

		// Copy image to buffered image
		Graphics g = bimage.createGraphics();

		// Paint the image onto the buffered image
		g.drawImage(image, 0, 0, null);
		g.dispose();

		return bimage;
	}

	public static BufferedImage toOpaque(BufferedImage src) {
		int w = src.getWidth();
		int h = src.getHeight();
		BufferedImage dest = new BufferedImage(w, h, BufferedImage.TYPE_INT_ARGB);
		int[] alpha = new int[w*h];
		for (int i=0; i<alpha.length; ++i)
			alpha[i] = Transparency.OPAQUE;
		int buf[] = new int[w];
		for (int y = 0; y < h; y++) {
			src.getRGB(0, y, w, 1, buf, 0, w);
			dest.setRGB(0, y, w, 1, buf, 0, w);
		}
		dest.getAlphaRaster().setPixels(0, 0, w, h, alpha);
//		GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
//		GraphicsConfiguration gc = ge.getDefaultScreenDevice().getDefaultConfiguration();
//		BufferedImage dest = gc.createCompatibleImage(w, h, Transparency.BITMASK);
//		int buf[] = new int[w];
//		for (int y = 0; y < h; y++) {
//		src.getRGB(0, y, w, 1, buf, 0, w);
//		dest.setRGB(0, y, w, 1, buf, 0, w);
//		}
		return dest;
	}

	/**
	 * Converts a BufferedImage to a BufferedImage with an alpha channel. If the given image already
	 * has an alpha channel, just returns it.
	 * @param src the source image
	 * @return an image with an alpha channel (opaque by default)
	 */
	public static BufferedImage toARGB(BufferedImage src) {
		if (hasAlpha(src))
			return src;
		int w = src.getWidth();
		int h = src.getHeight();
		BufferedImage dest = new BufferedImage(w, h, BufferedImage.TYPE_INT_ARGB);
		dest.setRGB(0, 0, w, h, src.getRGB(0, 0, w, h, null, 0, w), 0, w);
		return dest;
	}

	/**
	 * Convert pixels from java default ARGB int format to byte array in RGBA format.
	 * @param pixels the given pixels
	 * @return
	 */
	public static void convertARGBtoRGBA(int[] pixels)
	{
		int p, r, g, b, a;
		for (int i = 0; i < pixels.length; i++) {
			p = pixels[i];
			a = (p >> 24) & 0xFF;  // get pixel bytes in ARGB order
			r = (p >> 16) & 0xFF;
			g = (p >> 8) & 0xFF;
			b = (p >> 0) & 0xFF;
//			opixels[i] = r | (g << 8) | (b << 16) | (a << 24);
			pixels[i] = (a << 24) | (b << 16) | (g << 8) | (r << 0);
		}
	}

	/**
	 * Convert pixels from java default ARGB int format to byte array in RGBA format.
	 * @param jpixels
	 * @return
	 */
	public static void convertRGBAtoARGB(int[] pixels)
	{
		int p, r, g, b, a;
		for (int i = 0; i < pixels.length; i++) {
			p = pixels[i];
			a = (p >> 24) & 0xFF;  // get pixel bytes in ARGB order
			b = (p >> 16) & 0xFF;
			g = (p >> 8) & 0xFF;
			r = (p >> 0) & 0xFF;
//			opixels[i] = a | (r << 8) | (g << 16) | (b << 24);
			pixels[i] = (a << 24) | (r << 16) | (g << 8) | (b << 0);
		}
	}

	/**
	 * Returns true if the specified image has transparent pixels.
	 * 
	 * @param image the image to test
	 * @return true if the specified image has transparent pixels
	 */
	public static boolean hasAlpha(Image image) {
		// If buffered image, the color model is readily available
		if (image instanceof BufferedImage) {
			BufferedImage bimage = (BufferedImage)image;
			return bimage.getColorModel().hasAlpha();
		}

		// Use a pixel grabber to retrieve the image's color model;
		// grabbing a single pixel is usually sufficient
		PixelGrabber pg = new PixelGrabber(image, 0, 0, 1, 1, false);
		try {
			pg.grabPixels();
		} catch (InterruptedException e) {
		}

		// Get the image's color model
		ColorModel cm = pg.getColorModel();
		return cm.hasAlpha();
	}

}
