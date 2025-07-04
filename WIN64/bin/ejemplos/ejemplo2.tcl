# File: Cantilever2D_Main.tcl (use with Cantilever2D_Worker.tcl)
# Units: [kip,in.]
#
# Written: Andreas Schellenberg (andreas.schellenberg@gmail.com)
# Created: 09/07
# Revision: A
#
# Purpose: this file contains the tcl input to perform
# a hybrid simulation of a simple cantilever column
# with one experimental beamColumn element.
# Since the simulation of the column is performed in an
# other FE-software, the SimFEAdapter experimental control
# is used to communicate with such software.


# ------------------------------
# Start of model generation
# ------------------------------
logFile "Cantilever2D_Main.log"
defaultUnits -force kip -length in -time sec -temp F

# create ModelBuilder (with two-dimensions and 2 DOF/node)
model BasicBuilder -ndm 2 -ndf 3

# Load OpenFresco package
# -----------------------
# (make sure all dlls are in the same folder as openSees.exe)
# loadPackage OpenFresco

# Define geometry for model
# -------------------------
# node $tag $xCrd $yCrd $mass
node  1     0.0    0.0
node  2     0.0   54.0  -mass 0.08 0.08 0.0

# set the boundary conditions
# fix $tag $DX $DY $RZ
fix 1   1  1  1
fix 2   0  0  0

# Define control points
# ---------------------
# expControlPoint $cpTag <-node $nodeTag> $dof $rspType <-fact $f> <-lim $l $u> <-isRel> ...
expControlPoint 1  1 disp
expControlPoint 2  1 disp  1 force 

# Define experimental control
# ---------------------------
# expControl SimFEAdapter $tag ipAddr $ipPort -trialCP $cpTags -outCP $cpTags
expControl SimFEAdapter 1 "127.0.0.1" 44000 -trialCP 1 -outCP 2

# Define experimental setup
# -------------------------
# expSetup OneActuator $tag <-control $ctrlTag> $dof -sizeTrialOut $t $o <-trialDispFact $f> ...
expSetup OneActuator 1 -control 1 2 -sizeTrialOut 3 3 -ctrlDispFact -1.0 -daqDispFact -1.0 -daqForceFact -1.0

# Define experimental site
# ------------------------
# expSite LocalSite $tag $setupTag
expSite LocalSite 1 1

# Define coordinate transformation
# --------------------------------
# geomTransf Linear $transfTag
geomTransf Linear 1

# Define experimental element
# ---------------------------
# expElement beamColumn $eleTag $iNode $jNode $transTag �site $siteTag �initStif $Kij ... <�iMod> <�rho $rho>
expElement beamColumn 1 1 2 1 -site 1 -initStif 1213 0 0  0 11.2 -302.4  0 -302.4 10886.4

# Define dynamic loads
# --------------------
# set time series to be passed to uniform excitation
set dt 0.02
set scale 1.0
timeSeries Path 1 -filePath elcentro.txt -dt $dt -factor [expr 386.1*$scale]

# create UniformExcitation load pattern
# pattern UniformExcitation $tag $dir -accel $tsTag <-vel0 $vel0>
pattern UniformExcitation 1 1 -accel 1

# calculate the Rayleigh damping factors for nodes & elements
set alphaM     1.010017396536;  # D = alphaM*M
set betaK      0.0;             # D = betaK*Kcurrent
set betaKinit  0.0;             # D = beatKinit*Kinit
set betaKcomm  0.0;             # D = betaKcomm*KlastCommit

# set the Rayleigh damping 
#rayleigh $alphaM $betaK $betaKinit $betaKcomm
# ------------------------------
# End of model generation
# ------------------------------


