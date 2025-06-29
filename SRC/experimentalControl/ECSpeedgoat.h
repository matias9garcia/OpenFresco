/* ****************************************************************** **
**    OpenFRESCO - Open Framework                                     **
**                 for Experimental Setup and Control                 **
**                                                                    **
**                                                                    **
** Copyright (c) 2006, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited. See    **
** file 'COPYRIGHT_UCB' in main directory for information on usage    **
** and redistribution, and for a DISCLAIMER OF ALL WARRANTIES.        **
**                                                                    **
** Developed by:                                                      **
**   Andreas Schellenberg (andreas.schellenberg@gmx.net)              **
**   Yoshikazu Takahashi (yos@catfish.dpri.kyoto-u.ac.jp)             **
**   Gregory L. Fenves (fenves@berkeley.edu)                          **
**   Stephen A. Mahin (mahin@berkeley.edu)                            **
**                                                                    **
** ****************************************************************** */

#ifndef ECSpeedgoat_h
#define ECSpeedgoat_h

// Written: Andreas Schellenberg (andreas.schellenberg@gmail.com)
// Created: 09/24
// Revision: A
//
// Description: This file contains the class definition for ECSpeedgoat.
// ECSpeedgoat is a controller class for communicating with a Speedgoat
// target real-time digital signal processor.

#include "ExperimentalControl.h"

class ExperimentalCP;

class ECSpeedgoat : public ExperimentalControl
{
public:
    // constructors
    ECSpeedgoat(int tag,
        int nTrialCPs, ExperimentalCP **trialCPs,
        int nOutCPs, ExperimentalCP **outCPs,
        char *ipAddress, int ipPort = 22222,
        int udp = 0);
    ECSpeedgoat(const ECSpeedgoat&ec);
    
    // destructor
    virtual ~ECSpeedgoat();
    
    // method to get class type
    const char *getClassType() const {return "ECSpeedgoat";};
    
    // public methods to set and to get response
    virtual int setup();
    virtual int setSize(ID sizeT, ID sizeO);
    
    virtual int setTrialResponse(
        const Vector* disp,
        const Vector* vel,
        const Vector* accel,
        const Vector* force,
        const Vector* time);
    virtual int getDaqResponse(
        Vector* disp,
        Vector* vel,
        Vector* accel,
        Vector* force,
        Vector* time);
    
    virtual int commitState();
    
    virtual ExperimentalControl *getCopy();
    
    // public methods for experimental control recorder
    virtual Response *setResponse(const char **argv, int argc,
        OPS_Stream &output);
    virtual int getResponse(int responseID, Information &info);
    
    // public methods for output
    void Print(OPS_Stream &s, int flag = 0);
    
protected:
    // protected methods to set and to get response
    virtual int control();
    virtual int acquire();
    
private:
    int numTrialCPs;            // number of trial control points
    ExperimentalCP **trialCPs;  // trial control points
    int numOutCPs;              // number of output control points
    ExperimentalCP **outCPs;    // output control points
    char *ipAddress;            // ip-address of Speedgoat machine
    int ipPort;                 // ip-port of Speedgoat machine
    int udp;                    // udp socket flag
    const int dataSize;         // data size of network transactions
    
    Channel* theChannel;        // channel
    double* sData;              // send data array
    Vector* sendData;           // send vector
    double* rData;              // receive data array
    Vector* recvData;           // receive vector
    
    double *newTarget, *switchPC, *atTarget;  // communication flags
    int numCtrlSignals, numDaqSignals;        // number of signals
    Vector *ctrlSignal, *daqSignal;           // signal arrays
    Vector trialSigOffset;                    // trial signal offsets
    Vector ctrlSigOffset, daqSigOffset;       // ctrl and daq signal offsets (i.e. setpoints)
    int gotRelativeTrial;                     // relative trial signal flags
};

#endif
