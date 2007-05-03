/* An actor that reads an image from a FileParameter and outputs a TextureToken.
 * 
 * Copyright (c) 2006 Jean-Sebastien Senecal (js@drone.ws)
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
package drone.jogl.actor;

import java.awt.Image;
import java.io.IOException;
import java.net.URL;

import javax.swing.ImageIcon;

import com.sun.opengl.util.texture.TextureData;
import com.sun.opengl.util.texture.TextureIO;

import drone.jogl.data.TextureToken;

import ptolemy.actor.lib.Source;
import ptolemy.data.AWTImageToken;
import ptolemy.data.expr.FileParameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

//////////////////////////////////////////////////////////////////////////
//// ImageReader

/**
 This actor reads an Image from a FileParameter, and outputs it as an
 AWTImageToken.

 <p>FIXME: It would be nice if we could read images from stdin.

 @see FileParameter
 @see AWTImageToken
 @author  Christopher Hylands
 @version $Id: ImageReader.java,v 1.30 2005/07/08 19:56:22 cxh Exp $
 @since Ptolemy II 3.0
 @Pt.ProposedRating Red (cxh)
 @Pt.AcceptedRating Red (cxh)
 */
public class TextureReader extends Source {
    // We don't extend ptolemy.actor.lib.Reader because we are not
    // reading in data by columns.  Probably this class and
    // ptolemy.actor.lib.Reader should extend a common base class?

    /** Construct an actor with the given container and name.
     *  @param container The container.
     *  @param name The name of this actor.
     *  @exception IllegalActionException If the actor cannot be contained
     *   by the proposed container.
     *  @exception NameDuplicationException If the container already has an
     *   actor with this name.
     */
    public TextureReader(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);

        // Set the type of the output port.
        //output.setMultiport(true);
        output.setTypeEquals(BaseType.OBJECT);

        fileOrURL = new FileParameter(this, "fileOrURL");
    }

    ///////////////////////////////////////////////////////////////////
    ////                     ports and parameters                  ////

    /** The file name or URL from which to read.  This is a string with
     *  any form accepted by File Attribute.
     *  @see FileParameter
     */
    public FileParameter fileOrURL;

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** If the specified attribute is <i>URL</i>, then close
     *  the current file (if there is one) and open the new one.
     *  @param attribute The attribute that has changed.
     *  @exception IllegalActionException If the specified attribute
     *   is <i>URL</i> and the file cannot be opened.
     */
    public void attributeChanged(Attribute attribute)
            throws IllegalActionException {
        if (attribute == fileOrURL) {
            // Would it be worth checking to see if the URL exists and
            // is readable?
            _url = fileOrURL.asURL();
        }

        super.attributeChanged(attribute);
    }

    /** Output the data read in the prefire.
     *  @exception IllegalActionException If there's no director.
     */
    public void fire() throws IllegalActionException {
        super.fire();
        output.broadcast(new TextureToken(_textureData));
    }

    /** Open the file at the URL, and set the width of the output.
     */
    public void initialize() throws IllegalActionException {
        attributeChanged(fileOrURL);
    }

    /** Read in an image.
     *  @exception IllegalActionException If an IO error occurs.
     */
    public boolean prefire() throws IllegalActionException {
        if (_url == null) {
            throw new IllegalActionException("sourceURL was null");
        }
        
        try {
			_textureData = TextureIO.newTextureData(_url, false, null);
		} catch (IOException e) {
			throw new IllegalActionException(this, e, "Cannot open texture data from file " + _url.toString());
		}


//        if ((_image.getWidth(null) == -1) && (_image.getHeight(null) == -1)) {
//            throw new IllegalActionException(this,
//                    "Image size is -1 x -1.  Failed to open '" + _fileRoot
//                            + "'");
//        }

        return super.prefire();
    }

    ///////////////////////////////////////////////////////////////////
    ////                         private members                   ////
    
    // Image that is read in.
    private TextureData _textureData;

    // The URL of the file.
    private URL _url;
}
