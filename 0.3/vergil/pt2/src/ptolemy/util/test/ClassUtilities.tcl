# Tests for the ClassUtilities class
#
# @Author: Christopher Brooks
#
# @Version: $Id: ClassUtilities.tcl,v 1.4 2005/02/28 20:01:27 cxh Exp $
#
# @Copyright (c) 2003-2005 The Regents of the University of California.
# All rights reserved.
#
# Permission is hereby granted, without written agreement and without
# license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the
# above copyright notice and the following two paragraphs appear in all
# copies of this software.
#
# IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
# THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
# PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
# CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
# ENHANCEMENTS, OR MODIFICATIONS.
#
# 						PT_COPYRIGHT_VERSION_2
# 						COPYRIGHTENDKEY
#######################################################################

# Load the test definitions.
if {[string compare test [info procs test]] == 1} then {
    source testDefs.tcl
} {}

# Uncomment this to get a full report, or set in your Tcl shell window.
# set VERBOSE 1


######################################################################
####
#
test ClassUtilities-1.1 {lookupClassAsResource} {
    set resource [java::call ptolemy.util.ClassUtilities \
	    lookupClassAsResource \
	    "ptolemy.util.ClassUtilities"]
    # Check that the value returned starts with the value of PTII.
    # If $PTII has spaces in it, substitute spaces for %20.
    regsub {%20} $resource { } resource2
    # This might fail if ClassUtilities was found in a jar file
    regexp $resource2 $PTII
} {1}

######################################################################
####
#
test ClassUtilities-1.2 {lookupClassAsResource on a non-existant class} {
    set resource [java::call -noconvert \
	    ptolemy.util.ClassUtilities \
 	    lookupClassAsResource \
	    "Foo"]
    java::isnull $resource
} {1}
