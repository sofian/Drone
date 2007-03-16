package drone.frei0r.jni;

import java.awt.Point;
import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.awt.image.DataBufferInt;
import java.awt.image.Raster;
import java.net.URL;

import drone.frei0r.Frei0rException;

/**
 * Provide a bridge for Frei0r, minimalistic plugin API for video effects.
 * http://www.piksel.org/frei0r/1.0/spec/index.html
 * 
 * @author Mathieu Guindon, Jean-Sebastien Senecal
 */
public class Frei0r {
	
	/** one input and one output */
	public static final int F0R_PLUGIN_TYPE_FILTER = 0;
	/** just one output */
	public static final int F0R_PLUGIN_TYPE_SOURCE = 1;
	/** two inputs and one output */
	public static final int F0R_PLUGIN_TYPE_MIXER2 = 2;
	/** three inputs and one output */
	public static final int F0R_PLUGIN_TYPE_MIXER3 = 3;


	public Frei0r(String frei0rLibName) throws Frei0rException {
		try {
			// Try to open it as is.
			_openLibrary(frei0rLibName);
		} catch (Frei0rException e) {
			// Try to get it as a resource.
			ClassLoader cl = this.getClass().getClassLoader();
			URL path = cl.getResource(frei0rLibName);
			if (path == null) {
				throw new Frei0rException("Cannot open given frei0r library " + frei0rLibName + 
											": cannot find path to resource and/or failed to open because: " + e.getMessage());
			}
			_openLibrary(path.getFile());
		}
		if (_handle == 0)
			throw new Frei0rException("Handle not set.");
	}

	protected void finalize() throws Frei0rException {
		deinit();
		if (_handle != 0)
			_closeLibrary();
	}

	public Instance createInstance(int width, int height) throws Frei0rException {
		return new Instance(this, width, height);
	}

	/**< The (short) name of the plugin                   */
	public native String getName() throws Frei0rException;
	
	/**< The plugin author                                */
	public native String getAuthor() throws Frei0rException;
	/** The plugin type
	 * \see PLUGIN_TYPE
	 */
	
	public native int getPluginType() throws Frei0rException;
	
    /**< The color model used                             */
	public native int getColorModel() throws Frei0rException;
	
	/**< The frei0r major version this plugin is built for*/
	public native int getFrei0rVersion() throws Frei0rException;
	
	 /**< The major version of the plugin                  */
	public native int getMajorVersion() throws Frei0rException;
	
	  /**< The minor version of the plugin                  */
	public native int getMinorVersion() throws Frei0rException;
	
    /**< The number of parameters of the plugin           */
	public native int nParams() throws Frei0rException;
	
	/**< An optional explanation string               */
	public native String getExplanation() throws Frei0rException;

	/**<The (short) name of the param */
	public native String getParamName(int index) throws Frei0rException;
	
	/**<The type (see the F0R_PARAM_* defines) */
	public native int getParamType(int index) throws Frei0rException;
	
	/**<Optional explanation (can be 0) */
	public native String getParamExplanation(int index) throws Frei0rException;
	
//	/**< The (short) name of the plugin                   */
//	public String getName() { return _name; }
//	
//	/**< The plugin author                                */
//	public String getAuthor() { return _author; }
//	/** The plugin type
//	 * \see PLUGIN_TYPE
//	 */
//	
//	public int getPluginType() { return _pluginType; }
//	
//    /**< The color model used                             */
//	public int getColorModel() { return _colorModel; }
//	
//	/**< The frei0r major version this plugin is built for*/
//	public int getFrei0rVersion() { return _frei0rVersion; }
//	
//	 /**< The major version of the plugin                  */
//	public int getMajorVersion() { return _majorVersion; }
//	
//	  /**< The minor version of the plugin                  */
//	public int getMinorVersion() { return _minorVersion; }
//	
//    /**< The number of parameters of the plugin           */
//	public int nParams() { return _numParams; }
//	
//	/**< An optional explanation string               */
//	public String getExplanation() { return _explanation; }

	// /////////////////////////////////////////////////////////////////
	// // JNI section ////

	static {
		System.loadLibrary("Frei0rJNI");
	}


	/**
	 * Creates a handle to a frei0r dynamic library.
	 * @param libName the full path to the dynamic library
	 * @return a handle to the library
	 */
	protected native void _openLibrary(String libName) throws Frei0rException;

	protected native void _closeLibrary();

	// TODO: these methods should be protected
	public native int init() throws Frei0rException;

	public native void deinit() throws Frei0rException;

	public native long construct(int width, int height) throws Frei0rException;

	public native void destruct(long instance) throws Frei0rException;

	public native void update(long instance, double time, int[] inframe, int[] outframe) throws Frei0rException;

	public native void update2(long instance, double time,
			int[] inframe1, int[] inframe2, int[] inframe3,
			int[] outframe) throws Frei0rException;

	public native long getParamInfo(int paramIndex) throws Frei0rException;

	public native void setParamValue(long instance, long param, int paramIndex) throws Frei0rException;

	public native void getParamValue(long instance, long param, int paramIndex) throws Frei0rException;

	// XXX: there should be different instance types for different plugins (eg. source, mixer2, mixer3)
	public class Instance {

		protected Instance(Frei0r parent, int width, int height) throws Frei0rException {
			if (width < 0 || height < 0)
				throw new Frei0rException("Wrong dimensions (negative value): " + width + "x" + height);
			_width = width;
			_height = height;
			_instanceHandle = parent.construct(width, height);
			_parent = parent;
		}

		protected void finalize() throws Frei0rException {
			if (_instanceHandle != 0)
				_parent.destruct(_instanceHandle);
		}

		public int getWidth() { return _width; }

		public int getHeight() { return _height; }
		
		public Frei0r getParent() { return _parent; }

		public void update(double time, BufferedImage in, BufferedImage out) throws Frei0rException {

			// Check dimensions.
			if (_width != in.getWidth() || _height != in.getHeight())
				throw new Frei0rException("Input image has wrong dimensions: " +
						in.getWidth() + "x" + in.getHeight() + ", should be " +
						_width + "x" + _height);
			if (_width != out.getWidth() || _height != out.getHeight())
				throw new Frei0rException("Output image has wrong dimensions: " +
						out.getWidth() + "x" + out.getHeight() + ", should be " +
						_width + "x" + _height);

			// Extract input data buffer.
			DataBuffer bufferIn = in.getData().getDataBuffer();
			if (bufferIn.getDataType() != DataBuffer.TYPE_INT) {
				throw new Frei0rException("Data buffer of input image has wrong type.");
			}

			// Output frame buffer.
			int[] outframe = new int[bufferIn.getSize()];

			// Update frames.
			update(time, ((DataBufferInt)bufferIn).getData(), outframe);

			// Copy data to the output image buffer.
			// XXX it would be nice to find a way NOT to copy the data
			out.setRGB(0, 0, _width, _height, outframe, 0, _width);
		}

		public void update(double time, int[] inframe, int[] outframe) throws Frei0rException {
			_parent.update(_instanceHandle, time, inframe, outframe);
		}

		public void update2(double time, BufferedImage in1, BufferedImage in2,
								BufferedImage in3, BufferedImage out) throws Frei0rException {

			// Check dimensions.
			if (_width != in1.getWidth() || _height != in1.getHeight())
				throw new Frei0rException("Input image 1 has wrong dimensions: " +
						in1.getWidth() + "x" + in1.getHeight() + ", should be " +
						_width + "x" + _height);
			if (_width != in2.getWidth() || _height != in2.getHeight())
				throw new Frei0rException("Input image 2 has wrong dimensions: " +
						in2.getWidth() + "x" + in2.getHeight() + ", should be " +
						_width + "x" + _height);
			if (_width != in3.getWidth() || _height != in3.getHeight())
				throw new Frei0rException("Input image 3 has wrong dimensions: " +
						in3.getWidth() + "x" + in3.getHeight() + ", should be " +
						_width + "x" + _height);
			if (_width != out.getWidth() || _height != out.getHeight())
				throw new Frei0rException("Output image has wrong dimensions: " +
						out.getWidth() + "x" + out.getHeight() + ", should be " +
						_width + "x" + _height);

			// Extract input data buffer.
			DataBuffer bufferIn1 = in1.getData().getDataBuffer();
			if (bufferIn1.getDataType() != DataBuffer.TYPE_INT) {
				throw new Frei0rException("Data buffer of input image 1 has wrong type.");
			}
			DataBuffer bufferIn2 = in2.getData().getDataBuffer();
			if (bufferIn2.getDataType() != DataBuffer.TYPE_INT) {
				throw new Frei0rException("Data buffer of input image 2 has wrong type.");
			}
			DataBuffer bufferIn3 = in3.getData().getDataBuffer();
			if (bufferIn3.getDataType() != DataBuffer.TYPE_INT) {
				throw new Frei0rException("Data buffer of input image 3 has wrong type.");
			}

			// Output frame buffer.
			int[] outframe = new int[bufferIn1.getSize()];

			// Update frames.
			update2(time, ((DataBufferInt)bufferIn1).getData(),  ((DataBufferInt)bufferIn2).getData(),
					 ((DataBufferInt)bufferIn3).getData(), outframe);

			// Copy data to the output image buffer.
			// XXX it would be nice to find a way NOT to copy the data
			out.setRGB(0, 0, _width, _height, outframe, 0, _width);
		}

		public void update2(double time, int[] inframe1, int[] inframe2, int[] inframe3, int[] outframe) throws Frei0rException {
			_parent.update2(_instanceHandle, time, inframe1, inframe2, inframe3, outframe);
		}
		
		private long _instanceHandle;
		private Frei0r _parent;
		private int _width, _height;
	}

	// /////////////////////////////////////////////////////////////////
	// // private variables ////

	/**
	 * Used by native code exclusively. Handle to the dynamic library.
	 */
	@SuppressWarnings("unused")
	private long _handle;

//	// Plugin info.
//	private String _name;    /**< The (short) name of the plugin                   */
//	private String _author;  /**< The plugin author                                */
//	/** The plugin type
//	 * \see PLUGIN_TYPE
//	 */
//	private int _pluginType;    
//	private int _colorModel;     /**< The color model used                             */
//	private int _frei0rVersion;  /**< The frei0r major version this plugin is built for*/
//	private int _majorVersion;   /**< The major version of the plugin                  */
//	private int _minorVersion;   /**< The minor version of the plugin                  */
//	private int _numParams;      /**< The number of parameters of the plugin           */
//	private String _explanation; /**< An optional explanation string               */
}