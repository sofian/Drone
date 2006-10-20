# Tests for the BooleanToken class
#
# @Author: Neil Smyth, Yuhong Xiong
#
# @Version $Id: BooleanToken.tcl,v 1.26 2005/02/28 20:56:35 cxh Exp $
#
# @Copyright (c) 1998-2005 The Regents of the University of California.
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
test BooleanToken-1.0 {Create an empty instance} {
    set p [java::new ptolemy.data.BooleanToken]
    $p toString
} {false}

######################################################################
####
# 
test BooleanToken-1.1 {Create an instance with a value} {
    set token [java::new {ptolemy.data.BooleanToken boolean} true]
    $token toString
} {true}

######################################################################
####
# 
test BooleanToken-1.2 {Create an instance from a string value} {
    set token [java::new {ptolemy.data.BooleanToken String} "true"]
    $token toString
} {true}

######################################################################
####
# 
test BooleanToken-2.0 {Create a non-empty instance and query its value} {
    set token [java::new {ptolemy.data.BooleanToken boolean} false]
    $token booleanValue
} {0}

######################################################################
####
# 
test BooleanToken-3.0 {Create an non-empty instance and add it to Strings} {
    set token1 [java::new ptolemy.data.StringToken "value is " ]
    set token2 [java::new {ptolemy.data.BooleanToken boolean} true]
    set token3 [java::new ptolemy.data.StringToken "....." ]

    set token4 [$token1 add $token2]
    set token5 [$token2 add $token3]
    
    set token6 [$token4 add $token5]

    list [$token6 toString]
} {{"value is truetrue....."}}

######################################################################
####
# 
test BooleanToken-4.0 {Test addition of booleans} {
    set trueToken [java::field ptolemy.data.BooleanToken TRUE]
    set falseToken [java::field ptolemy.data.BooleanToken FALSE]
    set r1 [$falseToken add $falseToken]
    set r2 [$trueToken add $falseToken]
    set r3 [$falseToken add $trueToken]
    set r4 [$trueToken add $trueToken]
    list [$r1 toString] [$r2 toString] [$r3 toString] [$r4 toString]
} {false true true true}

test BooleanToken-4.1 {Test reverse addition of booleans} {
    set r1 [$falseToken addReverse $falseToken]
    set r2 [$trueToken addReverse $falseToken]
    set r3 [$falseToken addReverse $trueToken]
    set r4 [$trueToken addReverse $trueToken]
    list [$r1 toString] [$r2 toString] [$r3 toString] [$r4 toString]
} {false true true true}

test BooleanToken-5.0 {Test division of booleans} {
    set r1 [$falseToken divide $trueToken]
    set r2 [$trueToken divide $trueToken]
    list [$r1 toString] [$r2 toString]
} {false true}

test BooleanToken-5.1 {Test division by zero} {
    catch {[$falseToken divide $falseToken]} msg1
    catch {[$trueToken divide $falseToken]} msg2
    list $msg1 $msg2
} {{ptolemy.kernel.util.IllegalActionException: BooleanToken: division by false-valued token (analogous to division by zero).} {ptolemy.kernel.util.IllegalActionException: BooleanToken: division by false-valued token (analogous to division by zero).}}

test BooleanToken-5.2 {Test reverse division of booleans} {
    set r3 [$trueToken divideReverse $falseToken]
    set r4 [$trueToken divideReverse $trueToken]
    list [$r1 toString] [$r2 toString]
} {false true}

test BooleanToken-5.3 {Test division by zero} {
    catch {[$falseToken divideReverse $falseToken]} msg1
    catch {[$falseToken divideReverse $trueToken]} msg2
    list $msg1 $msg2
} {{ptolemy.kernel.util.IllegalActionException: BooleanToken: division by false-valued token (analogous to division by zero).} {ptolemy.kernel.util.IllegalActionException: BooleanToken: division by false-valued token (analogous to division by zero).}}


test BooleanToken-6.0 {Test equality test} {
    set r1 [$falseToken isEqualTo $falseToken]
    set r2 [$trueToken isEqualTo $falseToken]
    set r3 [$falseToken isEqualTo $trueToken]
    set r4 [$trueToken isEqualTo $trueToken]
    list [$r1 toString] [$r2 toString] [$r3 toString] [$r4 toString]
} {true false false true}


test BooleanToken-7.0 {Test multiplication} {
    set r1 [$falseToken multiply $falseToken]
    set r2 [$trueToken multiply $falseToken]
    set r3 [$falseToken multiply $trueToken]
    set r4 [$trueToken multiply $trueToken]
    list [$r1 toString] [$r2 toString] [$r3 toString] [$r4 toString]
} {false false false true}

test BooleanToken-7.1 {Test reverse multiplication} {
    set r1 [$falseToken multiplyReverse $falseToken]
    set r2 [$trueToken multiplyReverse $falseToken]
    set r3 [$falseToken multiplyReverse $trueToken]
    set r4 [$trueToken multiplyReverse $trueToken]
    list [$r1 toString] [$r2 toString] [$r3 toString] [$r4 toString]
} {false false false true}

test BooleanToken-8.0 {Test not} {
    set r1 [$falseToken not]
    set r2 [$trueToken not]
    list [$r1 toString] [$r2 toString]
} {true false}

test BooleanToken-9.0 {Test identities} {
    set r1 [$falseToken one]
    set r2 [$trueToken zero]
    list [$r1 toString] [$r2 toString]
} {true false}

test BooleanToken-10.0 {Test subtraction of booleans} {
    catch {set r1 [$falseToken subtract $falseToken]} errMsg
    list $errMsg
} {{ptolemy.kernel.util.IllegalActionException: subtract operation not supported between ptolemy.data.BooleanToken 'false' and ptolemy.data.BooleanToken 'false'}}

test BooleanToken-11.0 {Test reverse subtraction of booleans} {
    catch {set r1 [$falseToken subtract $falseToken]} errMsg
    list $errMsg
} {{ptolemy.kernel.util.IllegalActionException: subtract operation not supported between ptolemy.data.BooleanToken 'false' and ptolemy.data.BooleanToken 'false'}}



######################################################################
####
# 
test BooleanToken-12.0 {Test equals} {
    set t1 [java::new {ptolemy.data.BooleanToken boolean} true]
    set t2 [java::new {ptolemy.data.BooleanToken boolean} true]
    set f [java::new {ptolemy.data.BooleanToken boolean} false]
    list [$t1 equals $t1] [$t1 equals $t2] [$t1 equals $f]
} {1 1 0}

######################################################################
####
# 
test BooleanToken-12.1 {Test hashCode} {
    set t1 [java::new {ptolemy.data.BooleanToken boolean} true]
    set t2 [java::new {ptolemy.data.BooleanToken boolean} true]
    set f [java::new {ptolemy.data.BooleanToken boolean} false]
    list [$t1 hashCode] [$t2 hashCode] [$f hashCode]
} {1 1 0}

######################################################################
####
# 
test BooleanToken-13.0 {Test convert from BooleanToken} {
    set t [java::new {ptolemy.data.BooleanToken boolean} false]
    set msg {}
    set result {}
    catch {set result [[java::call ptolemy.data.BooleanToken convert $t] toString]} msg
    list $msg
} {false}

test BooleanToken-13.1 {Test convert from UnsignedByteToken} {
    set t [java::new {ptolemy.data.UnsignedByteToken byte} 1]
    set msg {}
    set result {}
    catch {set result [[java::call ptolemy.data.BooleanToken convert $t] toString]} msg
    list $msg
} {{ptolemy.kernel.util.IllegalActionException: Conversion is not supported from ptolemy.data.UnsignedByteToken '1ub' to the type boolean because the type of the token is higher or incomparable with the given type.}}

test BooleanToken-13.2 {Test convert from ComplexToken} {
    set o [java::new {ptolemy.math.Complex} 1.0 1.0]
    set t [java::new {ptolemy.data.ComplexToken ptolemy.math.Complex} $o]
    set msg {}
    set result {}
    catch {set result [[java::call ptolemy.data.BooleanToken convert $t] toString]} msg
    list $msg
} {{ptolemy.kernel.util.IllegalActionException: Conversion is not supported from ptolemy.data.ComplexToken '1.0 + 1.0i' to the type boolean because the type of the token is higher or incomparable with the given type.}}

test BooleanToken-13.3 {Test convert from DoubleToken} {
    set t [java::new {ptolemy.data.DoubleToken double} 1.0]
    set msg {}
    set result {}
    catch {set result [[java::call ptolemy.data.BooleanToken convert $t] toString]} msg
    list $msg
} {{ptolemy.kernel.util.IllegalActionException: Conversion is not supported from ptolemy.data.DoubleToken '1.0' to the type boolean because the type of the token is higher or incomparable with the given type.}}

test BooleanToken-13.4 {Test convert from FixToken} {
    set t [java::new {ptolemy.data.FixToken java.lang.String} "fix(1.0,8,4)"]
    set msg {}
    set result {}
    catch {set result [[java::call ptolemy.data.BooleanToken convert $t] toString]} msg
    list $msg
} {{ptolemy.kernel.util.IllegalActionException: Conversion is not supported from ptolemy.data.FixToken 'fix(1.0,8,4)' to the type boolean because the type of the token is higher or incomparable with the given type.}}

test BooleanToken-13.5 {Test convert from IntToken} {
    set t [java::new {ptolemy.data.IntToken int} 1]
    set msg {}
    set result {}
    catch {set result [[java::call ptolemy.data.BooleanToken convert $t] toString]} msg
    list $msg
} {{ptolemy.kernel.util.IllegalActionException: Conversion is not supported from ptolemy.data.IntToken '1' to the type boolean because the type of the token is higher or incomparable with the given type.}}

test BooleanToken-13.6 {Test convert from LongToken} {
    set t [java::new {ptolemy.data.LongToken long} 1]
    set msg {}
    set result {}
    catch {set result [[java::call ptolemy.data.BooleanToken convert $t] toString]} msg
    list $msg
} {{ptolemy.kernel.util.IllegalActionException: Conversion is not supported from ptolemy.data.LongToken '1L' to the type boolean because the type of the token is higher or incomparable with the given type.}}

test BooleanToken-13.7 {Test convert from StringToken} {
    set t [java::new {ptolemy.data.StringToken java.lang.String} "One"]
    set msg {}
    set result {}
    catch {set result [[java::call ptolemy.data.BooleanToken convert $t] toString]} msg
    list $msg
} {{ptolemy.kernel.util.IllegalActionException: Conversion is not supported from ptolemy.data.StringToken '"One"' to the type boolean because the type of the token is higher or incomparable with the given type.}}
    
