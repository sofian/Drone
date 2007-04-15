# Tests for the StringAttribute class
#
# @Author: Christopher Hylands
#
# @Version: $Id: StringAttribute.tcl,v 1.4 2005/02/28 20:22:23 cxh Exp $ 
#
# @Copyright (c) 2002-2005 The Regents of the University of California.
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

# Tycho test bed, see $PTII/doc/coding/testing.htm for more information.

# Load up the test definitions.
if {[string compare test [info procs test]] == 1} then {
    source testDefs.tcl
} {}

# Uncomment this to get a full report, or set in your Tcl shell window.
# set VERBOSE 1


######################################################################
####
#
test StringAttribute-1.0 {no arg Constructor} {
    set s1 [java::new ptolemy.kernel.util.StringAttribute]
    set output [java::new java.io.StringWriter]
    $s1 exportMoML $output 1
    list [$s1 toString] [$output toString]
} {{ptolemy.kernel.util.StringAttribute {.}} {    <property name="" class="ptolemy.kernel.util.StringAttribute">
    </property>
}}

test StringAttribute-1.1 {two arg Constructor} {
    set n [java::new ptolemy.kernel.util.NamedObj "my NamedObj"]
    set s2 [java::new ptolemy.kernel.util.StringAttribute $n "my StringAttribute"]
    set output [java::new java.io.StringWriter]
    $s2 exportMoML $output 1
    list [$s2 toString] [$output toString]
} {{ptolemy.kernel.util.StringAttribute {.my NamedObj.my StringAttribute}} {    <property name="my StringAttribute" class="ptolemy.kernel.util.StringAttribute">
    </property>
}}


test StringAttribute-2.1 {addValueListener} {
    set n [java::new ptolemy.kernel.util.NamedObj "my NamedObj"]
    set s2_1 [java::new ptolemy.kernel.util.StringAttribute $n "my StringAttribute"]
    set stream [java::new java.io.ByteArrayOutputStream]
    set printStream [java::new \
            {java.io.PrintStream java.io.OutputStream} $stream]
    set listener [java::new ptolemy.kernel.util.test.StreamValueListener \
	    $printStream]

    # Try removing the listener before adding it.
    $s2_1 removeValueListener $listener

    $s2_1 addValueListener $listener

    # Add the listener twice to get coverage of a basic block.
    $s2_1 addValueListener $listener

    $s2_1 setExpression "a string"

    # Remove the listener and verify that we are not updating it.
    $s2_1 removeValueListener $listener
    $s2_1 setExpression "another string"
    $printStream flush
    regsub -all [java::call System getProperty "line.separator"] \
	        [$stream toString] "\n" output
    list $output
} {{ptolemy.kernel.util.StringAttribute {.my NamedObj.my StringAttribute} changed, new expression: a string
}}


test StringAttribute-3.1 {getVisibility, setVisibility} {
    set s1 [java::new ptolemy.kernel.util.StringAttribute]
    set full [java::field ptolemy.kernel.util.Settable FULL]
    set r1 [expr {[$s1 getVisibility] == $full}]

    set none [java::field ptolemy.kernel.util.Settable NONE]
    $s1 setVisibility $none

    list $r1 [expr {[$s1 getVisibility] == $none}]
} {1 1}
