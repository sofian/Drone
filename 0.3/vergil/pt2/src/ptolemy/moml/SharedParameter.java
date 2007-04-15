/* A parameter that is shared globally in a model.

 Copyright (c) 2004-2006 The Regents of the University of California.
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
package ptolemy.moml;

import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;

import ptolemy.actor.ApplicationConfigurer;
import ptolemy.data.expr.Parameter;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.NamedObj;

//////////////////////////////////////////////////////////////////////////
//// SharedParameter

/**
 This parameter is shared throughout a model. Changing the expression of
 any one instance of the parameter will result in all instances that
 are shared being changed to the same expression.  An instance elsewhere
 in the model (within the same top level) is shared if it has the
 same name and its container is of the class specified in the
 constructor (or of the container class, if no class is specified
 in the constructor). Note that two parameters with the same
 expression do not necessarily have the same value, since the
 expression may reference other parameters that are in scope.
 <p>
 One exception is that if this parameter is (deeply) within an
 instance of EntityLibrary, then the parameter is not shared.
 Were this not the case, then opening a library containing this
 parameter would force expansion of all the sublibraries of
 EntityLibrary, which would defeat the lazy instantiation
 of EntityLibrary.
 <p>
 When this parameter is constructed, the specified container
 will be used to infer the parameter value from the container.
 That is, if the container is within a model that has any
 parameters shared with this one, then the value will be
 set to the last of those encountered.
 If the container is subsequently changed, it is up to the
 code implementing the change to use the inferValueFromContext()
 method to reset the value to match the new context.
 Note that this really needs to be done if the container
 of the container, or its container, or any container
 above this parameter is changed.  It is recommended to use
 the four-argument constructor, so you can specify a default
 value to use if there are no shared parameters.
 <p>
 Note that it might be tempting to use a static parameter field
 to achieve this effect, but this would be problematic for two
 reasons. First, the parameter would only be able to have one
 container. Second, the parameter would be shared across all
 models in the same Java virtual machine, not just within a
 single model.

 @author Edward A. Lee
 @version $Id: SharedParameter.java,v 1.24 2006/09/19 17:49:08 cxh Exp $
 @since Ptolemy II 4.1
 @Pt.ProposedRating Green (eal)
 @Pt.AcceptedRating Green (acataldo)
 */
public class SharedParameter extends Parameter {
    /** Construct a parameter with the given container and name.
     *  The container class will be used to determine which other
     *  instances of SharedParameter are shared with this one.
     *  NOTE: Do not use this constructor if you plan to set
     *  a default value. Use the four argument constructor instead.
     *  @param container The container.
     *  @param name The name of the parameter.
     *  @exception IllegalActionException If the parameter is not of an
     *   acceptable class for the container.
     *  @exception NameDuplicationException If the name coincides with
     *   a parameter already in the container.
     */
    public SharedParameter(NamedObj container, String name)
            throws IllegalActionException, NameDuplicationException {
        this(container, name, null, "");
    }

    /** Construct a parameter with the given container, name, and
     *  container class. The specified class will be used to determine
     *  which other instances of SharedParameter are shared with this one.
     *  NOTE: Do not use this constructor if you plan to set
     *  a default value. Use the four argument constructor instead.
     *  @param container The container.
     *  @param name The name of the parameter.
     *  @param containerClass The class used to determine shared instances.
     *  @exception IllegalActionException If the parameter is not of an
     *   acceptable class for the container.
     *  @exception NameDuplicationException If the name coincides with
     *   a parameter already in the container.
     */
    public SharedParameter(NamedObj container, String name, Class containerClass)
            throws IllegalActionException, NameDuplicationException {
        this(container, name, containerClass, "");
    }

    /** Construct a parameter with the given container, name,
     *  container class, and default value.  This is the preferred
     *  constructor to use.
     *  The specified class will be used to determine
     *  which other instances of SharedParameter are shared with this one.
     *  @param container The container.
     *  @param name The name of the parameter.
     *  @param containerClass The class used to determine shared instances.
     *  @param defaultValue The default value to use if the container's
     *   model has no shared parameters.
     *  @exception IllegalActionException If the parameter is not of an
     *   acceptable class for the container, or an empty string to specify no
     *   default value.
     *  @exception NameDuplicationException If the name coincides with
     *   a parameter already in the container.
     */
    public SharedParameter(NamedObj container, String name,
            Class containerClass, String defaultValue)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);

        if (containerClass == null) {
            containerClass = container.getClass();
        }

        _containerClass = containerClass;
        inferValueFromContext(defaultValue);
    }

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Return the top level of the containment hierarchy, unless
     *  one of the containers is an instance of EntityLibrary,
     *  in which case, return null.
     *  @return The top level, or null if this is within a library.
     */
    public NamedObj getRoot() {
        NamedObj result = this;

        while (result.getContainer() != null) {
            result = result.getContainer();

            // FIXME: this means that ptolemy.moml depends on
            // ptolemy.actor.gui.  We could either do instanceof
            // or else create ptolemy.kernel.ConfigurationBase
            // and have Configuration extend it.
            if (result instanceof ApplicationConfigurer) {
                // If the results is a Configuration, then go no higher.
                // If we do go higher, then we end up expanding the actor
                // library tree which take a long time and fails if
                // not all the actors are present.  For example, not
                // everyone will have Matlab or quicktime.
                return null;
            }
        }

        return result;
    }

    /** Infer the value of this parameter from the container
     *  context. That is, search for parameters that are
     *  shared with this one, and set the value of this parameter
     *  to match the first one encountered.
     *  If there are no shared parameters, then assign the
     *  default value given as an argument.
     *  @param defaultValue The default parameter value to give.
     */
    public void inferValueFromContext(String defaultValue) {
        Iterator sharedParameters = sharedParameterSet().iterator();
        while (sharedParameters.hasNext()) {
            SharedParameter candidate = (SharedParameter) sharedParameters.next();
            if (candidate != this) {
                defaultValue = candidate.getExpression();
                break;
            }
        }
        boolean previousSuppressing = _suppressingPropagation;
        try {
            _suppressingPropagation = true;
            setExpression(defaultValue);
        } finally {
            _suppressingPropagation = previousSuppressing;
        }
    }

    /** Return true if this instance is suppressing propagation.
     *  Unless setSuppressingPropagation() has been called, this
     *  returns false.
     *  @return Returns whether this instance is suppressing propagation.
     *  @see #setSuppressingPropagation(boolean)
     */
    public boolean isSuppressingPropagation() {
        return _suppressingPropagation;
    }

    /** Override the base class to also set the expression of shared
     *  parameters.
     *  @param expression The expression.
     */
    public void setExpression(String expression) {
        super.setExpression(expression);

        if (!_suppressingPropagation) {
            Iterator sharedParameters = sharedParameterSet().iterator();
            
            while (sharedParameters.hasNext()) {
                SharedParameter sharedParameter = (SharedParameter) sharedParameters.next();
                
                if (sharedParameter != this) {
                    try {
                        sharedParameter._suppressingPropagation = true;
                        
                        if (!sharedParameter.getExpression().equals(expression)) {
                            sharedParameter.setExpression(expression);
                        }
                    } finally {
                        sharedParameter._suppressingPropagation = false;
                    }
                }
            }
        }
    }

    /** Specify whether this instance should be suppressing
     *  propagation. If this is called with value true, then
     *  changes to this parameter will not propagate to other
     *  shared instances in the model.
     *  @param propagation True to suppress propagation.
     *  @see #isSuppressingPropagation()
     */
    public void setSuppressingPropagation(boolean propagation) {
        _suppressingPropagation = propagation;
    }

    /** Return a collection of all the shared parameters within the
     *  same model as this parameter.  If there are no such parameters
     *  or if this parameter is deeply contained within an EntityLibrary, then
     *  return an empty collection. The list will include this instance if
     *  this instance.
     *  A shared parameter is one that is an instance of SharedParameter,
     *  has the same name as this one, and is contained by the container
     *  class specified in the constructor.
     *  @return A list of parameters.
     */
    public synchronized Collection sharedParameterSet() {
        if (workspace().getVersion() != _sharedParameterSetVersion) {
            try {
                workspace().getReadAccess();
                _sharedParameterSet = new HashSet();
                _sharedParameterSetVersion = workspace().getVersion();
                NamedObj toplevel = getRoot();
                if (toplevel != null) {
                    _sharedParameterSet(toplevel, _sharedParameterSet);
                }
            } finally {
                workspace().doneReading();
            }
        }
        return _sharedParameterSet;
    }

    /** Override the base class to also validate the shared instances.
     *  @return A Collection of all the shared parameters within the same
     *   model as this parameter, see {@link #sharedParameterSet}.
     *  @exception IllegalActionException If this variable or a
     *   variable dependent on this variable cannot be evaluated (and is
     *   not lazy) and the model error handler throws an exception.
     *   Also thrown if the change is not acceptable to the container.
     */
    public Collection validate() throws IllegalActionException {
        Collection result = super.validate();
        if (result == null) {
            result = new HashSet();
        }

        // NOTE: This is called by setContainer(), which is called from
        // within a base class constructor. That call occurs before this
        // object has been fully constructed. It doesn't make sense at
        // that time to propagate validation to shared instances, since
        // in fact the value of this shared parameter will be inferred
        // from those instances if there are any. So in that case, we
        // just return.
        if (_containerClass == null) {
            return result;
        }

        if (!_suppressingPropagation) {
            Iterator sharedParameters = sharedParameterSet().iterator();
            while (sharedParameters.hasNext()) {
                SharedParameter sharedParameter = (SharedParameter) sharedParameters.next();
                if (sharedParameter != this) {
                    try {
                        sharedParameter._suppressingPropagation = true;
                        result.addAll(sharedParameter.validate());
                        result.add(sharedParameter);
                    } finally {
                        sharedParameter._suppressingPropagation = false;
                    }
                }
            }
        }
        return result;
    }

    ///////////////////////////////////////////////////////////////////
    ////                         protected methods                 ////

    /** Override the base class to do the propagation only if
     *  the specified destination is not shared, because if
     *  it is shared, then the value will be propagated
     *  in through the sharing mechanism.
     *  @param destination Object to which to propagate the
     *   value.
     *  @exception IllegalActionException If the value cannot
     *   be propagated.
     */
    protected void _propagateValue(NamedObj destination)
            throws IllegalActionException {
        if (!sharedParameterSet().contains(destination)) {
            super._propagateValue(destination);
        }
    }

    ///////////////////////////////////////////////////////////////////
    ////                         private methods                   ////

    /** Populate the specified list with
     *  all the shared parameters deeply contained by
     *  the specified container.  If there are no such parameters, then
     *  return an unmodified list. The list will include this instance if
     *  this instance is deeply contained by the specified container.
     *  A shared parameter is one that is an instance of SharedParameter,
     *  has the same name as this one, and is contained by the container
     *  class specified in the constructor.
     *  @param container The container.
     *  @param set The list to update.
     */
    private void _sharedParameterSet(NamedObj container, HashSet set) {
        // First check all the attributes of the specified container.
        if (_containerClass.isInstance(container)) {
            // If the attribute is not of the right class, get an exception.
            try {
                SharedParameter candidate = (SharedParameter)container.getAttribute(getName(),
                        SharedParameter.class);

                if (candidate != null) {
                    set.add(candidate);
                    // To avoid recronstructing the list again for each
                    // of the other shared parameters, we set its cache
                    // as well now.  It is for this reason that the calling
                    // method must be synchronized.
                    candidate._sharedParameterSet = set;
                    candidate._sharedParameterSetVersion = workspace().getVersion();
                }
            } catch (IllegalActionException ex) {
                // Ignore. Candidate doesn't match.
            }
        }
        Iterator containedObjects = container.containedObjectsIterator();
        while (containedObjects.hasNext()) {
            NamedObj candidateContainer = (NamedObj) containedObjects.next();
            _sharedParameterSet(candidateContainer, set);
        }
    }

    ///////////////////////////////////////////////////////////////////
    ////                         private variables                 ////

    /** The container class. */
    private Class _containerClass;
    
    /** Cached version of the shared parameter set. */
    private HashSet _sharedParameterSet;
    
    /** Version for the cache. */
    private long _sharedParameterSetVersion = -1L;

    /** Indicator to suppress propagation. */
    private boolean _suppressingPropagation = false;
}
