# Tests for the functionality of functions in UtilityFunctions class.
#
# @author: Neil Smyth
#
# @Version $Id: UtilityFunctions.tcl,v 1.29 2005/02/28 20:56:09 cxh Exp $
#
# @Copyright (c) 1997-2005 The Regents of the University of California.
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

# Ptolemy II test bed, see $PTII/doc/coding/testing.html for more information.

# Load up the test definitions.
if {[string compare test [info procs test]] == 1} then { 
    source testDefs.tcl
} {}

# Uncomment this to get a full report, or set in your Tcl shell window.
# set VERBOSE 1

# 
#


######################################################################
####
# 
test UtilityFunctions-1.0 {Check readFile method} {
    set parser [java::new ptolemy.data.expr.PtParser]
    
    set tree [$parser generateParseTree "readFile(\"message.txt\")"]

    #$tree displayParseTree " "
    set res [$tree evaluateParseTree]

    # This hack is necessary because of problems with crnl under windows
    regsub -all [java::call System getProperty "line.separator"] \
                [$res toString] "\n" output

    list $output
} {{"Greetings...
"}}

######################################################################
####
# 
test UtilityFunctions-1.1 {Check readFile method on a file that does not exist} {
    set parser [java::new ptolemy.data.expr.PtParser]
    set tree [$parser generateParseTree "readFile(\"not a file\")"]

    #$tree displayParseTree " "
    catch {set res [$tree evaluateParseTree]} msg
    # Use range here because the last part of the message varies
    # depending on the platform
    list [string range $msg 0 260]
} {{ptolemy.kernel.util.IllegalActionException: Error invoking function public static ptolemy.data.StringToken ptolemy.data.expr.UtilityFunctions.readFile(java.lang.String) throws ptolemy.kernel.util.IllegalActionException

Because:
File not found
Because:
not a fi}}

######################################################################
####
# result is 50 as the string for the re-invoked parser is 3+43+4 !
test UtilityFunctions-3.0 {Check recurive calls to the parser with eval method} {
    set parser [java::new ptolemy.data.expr.PtParser]
    
    set tree [$parser generateParseTree "eval(\"3 + 4\" + \"3 + 4\")"]

    set res [$tree evaluateParseTree]

    set value [$res toString]

    list $value
} {50}

######################################################################
####
test UtilityFunctions-4.0 {Test property} {
    set p1 [java::new ptolemy.data.expr.PtParser]
    set root [ $p1 {generateParseTree String} "property(\"file.separator\")"]
    set res  [ $root evaluateParseTree ]
    set fileSeparator [$res toString]
    set results "not ok"
    if { "$fileSeparator" == "\"/\"" || "$fileSeparator" == "\"\\\""} {
	set results "ok"
    }
    list $results
} {ok}

######################################################################
test UtilityFunctions-4.1 {Test property on a parameter that does not exist} {
    set p1 [java::new ptolemy.data.expr.PtParser]
    set root [ $p1 {generateParseTree String} "property(\"not a parameter\")"]
    set res  [ $root evaluateParseTree ]
    $res toString
} {""}

######################################################################
## MatrixParser tests
test UtilityFunctions-2.0 {Check up on matrices } {
    set parser [java::new ptolemy.data.expr.PtParser]   
    set tree [$parser generateParseTree "\[ 0.0, 3.0; 2.0,  0.0 \]"]
    set matrix [$tree evaluateParseTree]
    $matrix toString
} {[0.0, 3.0; 2.0, 0.0]}

test UtilityFunctions-3.0 {Check readmatrix method} {
    set parser [java::new ptolemy.data.expr.PtParser]   
    set tree [$parser generateParseTree "readMatrix('matrix.mat')"]
    set matrix [$tree evaluateParseTree]
    $matrix toString
} {[0.0, 4.0; 2.0, 0.0]}


test UtilityFunctions-3.1 {Check readMatrix method twice} {
    set parser [java::new ptolemy.data.expr.PtParser]   
    set tree [$parser generateParseTree "readMatrix('matrix.mat')"]
    set tree [$parser generateParseTree "readMatrix('matrix1.mat')"]
    set matrix [$tree evaluateParseTree]
    $matrix toString
} {[1.0, 2.0; 3.0, 4.0]}

######################################################################
####
test UtilityFunctions-5.1 {Test zeroMatrix} {
    set parser [java::new ptolemy.data.expr.PtParser]
    set tree [$parser generateParseTree "zeroMatrix(2,3)"]
    set matrix [$tree evaluateParseTree]
    $matrix toString
} {[0.0, 0.0, 0.0; 0.0, 0.0, 0.0]}

