/* An interface that find documentation.

 Copyright (c) 2006 The Regents of the University of California.
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
 */

package ptolemy.vergil.actor;

import java.net.URL;
import java.util.List;

import ptolemy.util.ExecuteCommands;

//////////////////////////////////////////////////////////////////////////
//// DocApplicationSpecializer

/**
 An interface that specializes the documentation system to the application.

 The docClassNameToURL() method used by {@link DocManager} and other
 classes to convert class names to URLs.  The buildCommands() method
 is used by {@link DocBuilder} to set up the build environment to
 build the documentation system if necessary.

 <p>If an application would like more control over how documentation
 is found and built, then the application can implement this interface and set
 the _docApplicationSpecializer parameter in the configuration to name
 the implementation class.

 @author Christopher Brooks
 @version $Id: DocApplicationSpecializer.java,v 1.7 2006/08/21 23:54:10 cxh Exp $
 @since Ptolemy II 5.2
 @Pt.ProposedRating Red (cxh)
 @Pt.AcceptedRating Red (cxh)
 */
public interface DocApplicationSpecializer {

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Given a dot separated class name, return the URL of the
     *  documentation.
     *  @param remoteDocumentationURLBase If non-null, the URL of the
     *  documentation.  Usually, this is set by reading the 
     *  _remoteDocumentationBase parameter from the configuration in the
     *  caller.
     *  @param className The dot separated class name.
     *  @param lookForPtDoc True if we should look for ptdoc .xml files.
     *  @param lookForJavadoc True if we should look for javadoc files. 
     *  @param lookForSource True if we should look for source files.
     *  @param lookForActorIndex True if we should look for the actor index.
     *  @return The URL of the documentation, if any.  If no documentation
     *  was found, return null.
     */
    public URL docClassNameToURL(String remoteDocumentationURLBase,
            String className, boolean lookForPtDoc, boolean lookForJavadoc,
            boolean lookForSource, boolean lookForActorIndex);

    /** Set up the commands necessary to build the documentation.
     *  @param executeCommands  The command execution environment necessary
     *  to build the documentation.
     *  @return A List of Strings, where each String represents the a
     *  command to be executed.
     */
    public List buildCommands(ExecuteCommands executeCommands);
}
