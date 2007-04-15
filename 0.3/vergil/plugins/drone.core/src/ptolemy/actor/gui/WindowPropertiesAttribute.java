/* An attribute representing the size, location, and other window properties.

 Copyright (c) 1998-2006 The Regents of the University of California.
 All rights reserved.
 Permission is hereby granted, without written agreement and without
 license or royalty fees, to use, copy, modify, and distribute this
 software and its documentation for any purpose, provided that the above
 copyright notice and the following two paragraphs appear in all copies
 of this software.

 IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
 FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

 THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
 PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
 CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.

 PT_COPYRIGHT_VERSION_2
 COPYRIGHTENDKEY

 */
package ptolemy.actor.gui;

import java.awt.Frame;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import ptolemy.data.ArrayToken;
import ptolemy.data.BooleanToken;
import ptolemy.data.IntToken;
import ptolemy.data.RecordToken;
import ptolemy.data.expr.Parameter;
import ptolemy.gui.Top;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.NamedObj;
import ptolemy.kernel.util.Settable;

//////////////////////////////////////////////////////////////////////////
//// WindowPropertiesAttribute

/**
 This attribute stores properties of a window, including the width,
 height, and location. The token in this attribute is a RecordToken
 containing a field "bounds" with a 4-element integer array.
 When we fully commit to Java 1.4, there will also be a field
 to indicate whether the window is maximized.
 By default, this attribute has visibility NONE, so the user will not
 see it in parameter editing dialogs.

 @author Edward A. Lee
 @version $Id: WindowPropertiesAttribute.java,v 1.31 2006/08/21 23:10:03 cxh Exp $
 @since Ptolemy II 2.1
 @Pt.ProposedRating Red (eal)
 @Pt.AcceptedRating Red (johnr)
 */
public class WindowPropertiesAttribute extends Parameter implements
        ComponentListener {
    /** Construct an attribute with the given name contained by the specified
     *  entity. The container argument must not be null, or a
     *  NullPointerException will be thrown.  This attribute will use the
     *  workspace of the container for synchronization and version counts.
     *  If the name argument is null, then the name is set to the empty string.
     *  Increment the version of the workspace.
     *  @param container The container.
     *  @param name The name of this attribute.
     *  @exception IllegalActionException If the attribute is not of an
     *   acceptable class for the container, or if the name contains a period.
     *  @exception NameDuplicationException If the name coincides with
     *   an attribute already in the container.
     */
    public WindowPropertiesAttribute(NamedObj container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);
        setVisibility(Settable.NONE);

        // The class mechanism tends to suppress export of this attribute.
        // Why?  We override that here.
        setPersistent(true);
    }

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Do nothing. This method is
     *  invoked when the component has been made invisible.
     *  @param event The component event.
     */
    public void componentHidden(ComponentEvent event) {
    }

    /** Record the new position. This method is
     *  invoked when the component's position changes.
     *  @param event The component event.
     */
    public void componentMoved(ComponentEvent event) {
        recordProperties(_listeningTo);
    }

    /** Record the new size. This method is
     *  invoked when the component's size changes.
     *  @param event The component event.
     */
    public void componentResized(ComponentEvent event) {
        recordProperties(_listeningTo);
    }

    /** Do nothing. This method is
     *  invoked when the component has been made visible.
     *  @param event The component event.
     */
    public void componentShown(ComponentEvent event) {
    }

    /** Set the value of the attribute to match those of the specified
     *  frame.
     *  @param frame The frame whose properties are to be recorded.
     */
    public void recordProperties(Frame frame) {
        try {
            Rectangle bounds = frame.getBounds();

            // Determine whether the window is maximized.
            boolean maximized = (frame.getExtendedState() & Frame.MAXIMIZED_BOTH) == Frame.MAXIMIZED_BOTH;

            // Construct values for the record token.
            setToken("{bounds={" + bounds.x + ", " + bounds.y + ", "
                    + bounds.width + ", " + bounds.height + "}, maximized="
                    + maximized + "}");
        } catch (IllegalActionException ex) {
            throw new InternalErrorException("Can't set propertes value! " + ex);
        }
    }

    /** Set the properties of the specified frame to match the
     *  current value of the attribute.  If the value of the attribute
     *  has not been set, then do nothing and return true. If the
     *  value of this attribute is malformed in any way, then just
     *  return false.
     *
     *  <p>If the x or y position is less than 0 or greater than the
     *  width or height height of the screen, then offset the position
     *  by 30 pixels so the user can drag the window.
     *
     *  @param frame The frame whose properties are to be set.
     *  @return True if successful.
     */
    public boolean setProperties(Frame frame) {
        if (_listeningTo != frame) {
            if (_listeningTo != null) {
                _listeningTo.removeComponentListener(this);
            }

            frame.addComponentListener(this);
            _listeningTo = frame;
        }

        try {
            RecordToken value = (RecordToken) getToken();

            if (value == null) {
                return true;
            }

            ArrayToken boundsToken = (ArrayToken) value.get("bounds");
            BooleanToken maximizedToken = (BooleanToken) value.get("maximized");
            int x = ((IntToken) boundsToken.getElement(0)).intValue();
            int y = ((IntToken) boundsToken.getElement(1)).intValue();
            int width = ((IntToken) boundsToken.getElement(2)).intValue();
            int height = ((IntToken) boundsToken.getElement(3)).intValue();

            // If x or y is less than 0 or greater than the width or
            // height of the screen, then offset them by 30 pixels so
            // the user can drag the window.
            // FIXME: If we change the size, should we mark the model
            // as dirty so it gets saved?
            // FIXME: will these changes cause problems with multiscreen
            // monitors?
            x = (x < 0 ? 30 : x);
            y = (y < 0 ? 30 : y);

            Toolkit tk = Toolkit.getDefaultToolkit();
            x = (x > tk.getScreenSize().width ? tk.getScreenSize().width - 30
                    : x);
            y = (y > tk.getScreenSize().height ? tk.getScreenSize().height - 30
                    : y);

            frame.setBounds(x, y, width, height);

            if (maximizedToken != null) {
                boolean maximized = maximizedToken.booleanValue();

                if (maximized) {
                    // FIXME: Regrettably, this doesn't make the window
                    // actually maximized under Windows, at least.
                    frame.setExtendedState(frame.getExtendedState()
                            | Frame.MAXIMIZED_BOTH);
                }
            }

            if (frame instanceof Top) {
                // Disable centering.
                ((Top) frame).setCentering(false);
            }

            return true;
        } catch (Exception ex) {
            return false;
        }
    }

    ///////////////////////////////////////////////////////////////////
    ////                         private variables                 ////

    /** The frame we are listening to. */
    private Frame _listeningTo;
}
