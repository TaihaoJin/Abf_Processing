#if !defined(AFX_EVLNODE_H__829A080D_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_EVLNODE_H__829A080D_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_
#include <vector>
using namespace std;
typedef vector <float> VectF;
typedef vector <int> nVect;
typedef vector <double> dVect;

struct structFRange {
	float fLeft;
	float fRight;
};

typedef vector <structFRange> VectFR;


struct EvlNode {
	int nNumEvents;
	const int* pnLevelStart;
	const int* pnLevelLength;
	const short* pstEventCurrentLevel;
	const float* pfAmplitude;
	const float* pfBaseline;
	float  fTimePerPoint;
	int nRecordLength;
};

struct structSegmentExtractionNode{
	float	fLength;
	float	fBegin;
	float	fEnd;
	int		nRefSweep;
	int		nFirstSweep;
	int		nLastWeep;
	bool	bSubstractReference;
	bool	bAlignMaximum;
};

struct structSegmentNode
{
	int nFirstPoint;
	int nLastPoint;
	int nLength;
	int nArtifactType;
	float fMaximum;
	float fFirstPoint;
	float fLastPoint;
};

struct structDwellFittingTrialNode
{
	CString sEvlFileName;
	int nDwellLevel;
	int nNumComponents;
	int nTrialNumber;
	double fWeights[20];
	double fTimeConstants[20];
	bool bFreezeW[20];
	bool bFreezeT[20];
	structFRange fFittingRange;
	float fConfidanceRange;
	bool  bConfidanceRange;
};

struct structFittingResultNode
{
	CString sFileName;
	CString sCurFileName;
	CString sBinMethod;
	int nLargestBin;
	int nDwellLevel;
	int nNumComponents;

	int nScoreRanking;
	double dScalings[20];
	double dWeights[20];
	double dTimeConstants[20];
	double dMean;
	double dLogLikelihood;
	double dLogLikelihood_R;

	double dSSE;
	double dChiSQ;
	double dSDev;
	double dMeanDwellTimeO;//The mean dwell time of the events observed in the fitted rage.
	double dMeanDwellTimeH;//The mean dwell time calculated from the eventlist based on hibrid bins.

	double dMeanDwellTimeHC;//The mean dwell time calculated from the fitting based on hibrid bins.
	double dMeanDwellTimeC;//The mean dwell time calculated by numerical integration of the fitting over the fitted range.
	int	  nNumEvents;
	int   nSignificantComponents;
	structFRange fFittingRange;
	structFRange fConfidanceRangeT[20];
	structFRange fConfidanceRangeW[20];
	structFRange fConfidanceRangeTL[20];
	structFRange fConfidanceRangeWL[20];
	structFRange fConfidanceRangeTI[20];
	structFRange fConfidanceRangeWI[20];
	structFRange fConfidanceRangeTLI[20];
	structFRange fConfidanceRangeWLI[20];
	float fConfidanceRange;
	bool  bConfidanceRange;

	int   nEventsInRange;
	int   nTrialNumber;
	double dFittedArea;
	int   nNumIterations;
	double dCorrection;

	int   nLargestLogBin[20];
	int   nLargestHybridBin[20];
	int   nFuncKey;//1 for Log, 2 for all-events and 3 for Hybrid
	int   nMaxLevel,nNumChannels;
	int   nClusterIndex;
	float fProbSingle,fMeanOpenTime,fOpenProb,fRecordLength;

	structDwellFittingTrialNode aTrialNode;
};

typedef vector <structDwellFittingTrialNode> VectTrials;
typedef vector <structFittingResultNode> VectResults;

typedef vector <VectTrials> TotalVTrials;
typedef vector <VectResults> TotalVResults;

typedef vector <TotalVResults> TotalVVResults;
typedef vector <int> VectInt;
//typedef vector <VectInt> VVectInt;

struct structFittingResultsCollectionNode
{
	CString sEvlFileName;
	int nNumFittingLevels;
	vector <int> vnFittingLevels;
	vector <int> vnSignificantComponents;
	vector <int> vnMinComponents;
	vector <int> vnMaxComponents;
	vector <VectInt> vvnNumTrials;
	TotalVVResults aTotalVVResults;
};

struct structSemNode
{
	float fMean;
	float fSem;
	float nSampleSize;
	float fMax;
	float fMin;
};

struct SegmentNode 
{
	int nLength;
	float fAmpBegin;
	float fAmpEnd;
	float *pfData;
	float fSlope;
};

struct AbfNode{
	float* pfData;
	int nSize;
	int nNumChannels;
	float fSampleInterval;
	float fAutosampleAdditGain;
	float fMinAmp,fMaxAmp;
	int nAbfID;
	CString sFileName;
};	

struct structAllPointHistogramNode{
	float fMin,fMax,fBase,fDelta;
	int nDim;
	float *pfHist;
};

struct structEvlHistogramNode{
	int   nLargestLogBin[20];
	int   nLargestHybridBin[20];
	structFRange fRDataRange[20];
	float fMinLinear;
	float fMinLog;
	float fMaxLinear;
	float fMaxLog;
	float fDeltaLinear;
	float fDeltaLog;
	float fRecordLength;
	int nNumEvents;
	int nMaxLevel;
	int nNumOfChannels;
	int nNumBinsLinear;
	int nNumBinsLog;
	int nEventsInRange_HB[20];
	int nEventsInRange_Log[20];
	vector <float*> pfvectHistLinear;
	vector <float*> pfvectHistLog;
	vector <VectF> vectFLogHist;
	vector <VectFR> vectFRLogBins;
	vector <VectF> vectFLogHist_HB;
	vector <VectFR> vectFRLogBins_HB;
	vector <VectF> vectfDwellLength;
};


struct struct2DEvlHistogramNode{
	float fMinLog;
	float fMaxLog;
	float fDeltaLog;
	float fRecordLength;
	float HistOC[110][110][3];//The first, second and third element are Counts, total open time and toltal closed-time, respectively.
							  //The first and the second indeces are the the following closed-time and the open time indeces, respectively.
	float HistCO[110][110][3];//The first, second and third element are Counts, total open time and toltal closed-time, respectively.
							  //The first and the second indeces are the the preceding closed-time and the open time indeces, respectively.
	float HistOMinC[110][110][3];//The first, second and third element are Counts, total open time and toltal closed-time, respectively.
							  //The first and the second indeces are the the smaller closed-time (of the preceding and the following closed events) and the open time indeces, respectively.
	float HistOMaxC[110][110][3];//The first, second and third element are Counts, total open time and toltal closed-time, respectively.
							  //The first and the second indeces are the the larger closed-time and the open time indeces, respectively.
	
	bool bNonEmptyRow[110];
	bool bNonEmptyColumn[110];

	float MeanToOC[110][3];//The first, second and third element are the mean open time of the open events with the following closed time 
					  //smaller, equal or >= the bin value (closed time), respectively.
	float MeanToCO[110][3];//The first, second and third element are the mean open time of the open events with the preceding closed time 
					  //smaller, equal or >= the bin value (closed time), respectively.
	float MeanToONC[110][3];//The first, second and third element are the mean open time of the open events with the smaller closed time
					  //smaller, equal or >= the bin value (closed time), respectively.

	float MeanToOXC[110][3];//The first, second and third element are the mean open time of the open events with the larger closed time 
					  //smaller, equal or >= the bin value (closed time), respectively.

	float SemToOC[110][3];//The first, second and third element are the sem of the open time of the open events with the following closed time 
					  //smaller, equal or >= the bin value (closed time), respectively.
	float SemToCO[110][3];//The first, second and third element are the sem of the open time of the open events with the preceding closed time 
					  //smaller, equal or >= the bin value (closed time), respectively.

	float SemToONC[110][3];//The first, second and third element are the sem of the open time of the open events with the smaller closed time 
					  //smaller, equal or >= the bin value (closed time), respectively.

	float SemToOXC[110][3];//The first, second and third element are the sem of the open time of the open events with the Larger closed time 
					  //smaller, equal or >= the bin value (closed time), respectively.

	float CountsToOC[110][3];
	float CountsToCO[110][3];
	float CountsToOXC[110][3];
	float CountsToONC[110][3];

	float MeanToCOC[110][110][2];
		  //ToCOC[i][j][0] and ToCOC[i][j][1] hold the total time and the number of the open events with the 
	      // left and right closed time in the range of i and j segments, respectively.
	float MeanToCOCNX[110][110][2];
		  //ToCOC[i][j][0] and ToCOC[i][j][1] hold the total time and the number of the open events with the 
	      // left closed-time smaller than ti and the right closed-time larger than tj, respectively.
	float MeanToCOCXN[110][110][2];
		  //ToCOC[i][j][0] and ToCOC[i][j][1] hold the total time and the number of the open events with the 
	      // left closed-time larger than ti and the right closed-time smaller than tj, respectively.

	float SemToCOC[110];
		  //ToCOC[i][j][0] and ToCOC[i][j][1] hold the total time and the number of the open events with the 
	      // left and right closed time in the range of i and j segments, respectively.
	float SemToCOCNX[110];
		  //ToCOC[i][j][0] and ToCOC[i][j][1] hold the total time and the number of the open events with the 
	      // left closed-time smaller than ti and the right closed-time larger than tj, respectively.
	float SemToCOCXN[110];
		  //ToCOC[i][j][0] and ToCOC[i][j][1] hold the total time and the number of the open events with the 
	      // left closed-time larger than ti and the right closed-time smaller than tj, respectively.

	int nNumEvents;
	int nMaxLevel;
	int nNumOfChannels;
	int nNumBinsLogO;
	int nNumBinsLogC;
	int nIndexLogOMax;
	int nIndexLogCMax;
	int nIndexLogOMin;
	int nIndexLogCMin;
};


struct structFittedParameterNode{
	CString sFittedParameter;
	int nFittedLevel;
	int nSigComponentsL;
	int nSigComponentsH;
	int nNumComponents;
	bool bOutputConfidanceRanges;
};

struct structExponentialFittingNode
{
	int nFuncKey;
	int nNumComponents;
	int nFreeParameters;
	int nIterations;
	double dWeights[20];
	double dLambda[20];
	double dTimeConstants[20];
	double dLogLik;
	bool bFreezeW[20];
	bool bFreezeL[20];
	double dArea;
	structFRange fFittingRange;
	int nNumTotalEvents;
	int nTrialNumber;
	int nScoreRanking;
	double dLogLik_R;
	double dMeanTCA;
};


struct structGeometricFittingNode
{
	int nFuncKey;
	int nTrialNumber;
	int nNumComponents;
	int nScoreRanking;
	int nFreeParameters;
	int nIterations;
	double dWeights[20];
	double dRol[20];
	double dLogLik;
	double dLogLik_R;
	bool bFreezeW[20];
	bool bFreezeR[20];
	double dArea;
	structFRange fFittingRange;
	int nNumTotalEvents;
	double dMeanCA;//Calculated Mean
};

typedef vector <structGeometricFittingNode> vGeoFittingNode;
struct structNumOpenPerBurstFittingNode
{
	float fBurstDelimiter;
	CString sEvlFileName;
	CString sCurveFileName;
	int nMinComponents;
	int nMaxComponents;
	vector <double> vpValue;
	vector <vGeoFittingNode> vTrialNodeCollections;
	vector <vGeoFittingNode> vResultNodeCollections;
	vector <int> vnNumOfTrials;
	int nNumSigComponents;
};

typedef vector <structExponentialFittingNode> vExpFittingNode;
struct structTOTPBFittingNode
{
	float fBurstDelimiter;
	CString sEvlFileName;
	int nBurstDelimieterIndex;
	CString sCurveFileName;
	int nMinComponents;
	int nMaxComponents;
	vector <double> vpValue;
	vector <vExpFittingNode> vTrialNodeCollections;
	vector <vExpFittingNode> vResultNodeCollections;
	vector <int> vnNumOfTrials;
	int nNumSigComponents;
};

struct structBSTFittingNode
{
	float fBurstDelimiter;
	int nBurstDelimieterIndex;
	CString sEvlFileName;
	CString sCurveFileName;
	int nMinComponents;
	int nMaxComponents;
	vector <double> vpValue;
	vector <vExpFittingNode> vTrialNodeCollections;
	vector <vExpFittingNode> vResultNodeCollections;
	vector <int> vnNumOfTrials;
	int nNumSigComponents;
};

struct structGammaFittingNode
{
	int nFuncKey;
	int nNumComponents;
	int nDataSize;
	int nScoreRanking;
	int nFreeParameters;
	double dRol0;
	double *pdData;
	double *dLik;
	double dWeights[20];
	double dRol[20];
	double dLogLik;
	bool bFreezeW[20];
	bool bFreezeR[20];
	double dH;
	double dArea;
	structFRange fFittingRange;
};


struct structGaussianFittingNode
{
	int nNumComponents;
	int nDataSize;
	int nFreeParameters;
	bool bFreezeDev;
	double *pdData;
	double *dLik;
	double dWeights[20];
	double dMean[20];
	double dDev[20];
	double dLogLik;
	int nWFix[20];
	int nMFix[20];
	int nDFix[20];
};

struct structBurstTCBinInfoNode{ 
	int nStartingBin; //The index of the time course bin (numbering from 0) in which burst started (the starting bin).
	int nEndingBin; //The index of the time course bin (numbering from 0) in which burst ended (the ending bin).
	int nNumOpenings0; //The number of openings started in the starting bin.
	int nNumOpenings1; //The number of openings started in the ending bin.
};

struct structBurstTCBinNode{ 
	int nFirstBurst; //The index (numbering from 0) the first burst starting or ending in the bin.
	int nLastBurst; //The index (numbering from 0) the last burst starting or ending in the bin.
	int nSpanningBurst; //The index (numbering from 0) of the burst spanning over the entire bin.
	float fTOT; //Total open time;
	float fPo; //Mean open probability, including only the proportion of the closed or open time falling into the bin.
	float fMFo; //Mean frequency of the openings, including only those openings started in the bin.
	int nNumBursts; //The total number of bursts biginning in the bin, spanning the bin, and ending in the bin.
	int nNumOpenings; //The total number of openings biginning in the bin, spanning the bin, and ending in the bin.
	float fMTo; //The mean open time, including all openings whose any parts falling into the bin.
	float fMBTo; //The mean burst duration, including all bursts whose any openings falling into the bin.
	float fNOPB;  //The mean number of openings per bursts, including all bursts, if any part of the burst fall into the bin, 
				  //therefore the bursts spanning the border will be counted in more than one time course bin.
	float fMTOTPB; //The mean total open time burst.
	float fMBTc; //The mean interburst interval.
	float fMPoib; //The mean intraburst open probability.
	float fMFob; //The frequency of bursting.
	float fMPob; //The burst probability.
};

struct structBurstNode{
	structBurstTCBinInfoNode structBinInfo;
	bool bValid;
	int nFirstOpening;
	int nLastOpening;
	int nNumOpenings;
	int nNumClosings;
	int nNumevents;
	float fTotalOpenTime;
	float fLength;  //The burst duration
	int nPrecedingIBC;		//The position (in the unit of event) of the preceding inter-burst interval
	float fPrecedingIBC;	//The length (ms) of the preceding inter-burst interval
};

struct structBurstNodes{
	double dTcb;//burst delimiter
	vector <structBurstNode> burstNodes;
	int nEvlID;
	int nBurstID;
};


struct structClusterNode{
	bool bValid;
	vector <structBurstNode> svBurstNodes;
	float fTotalOpenTime;
	float fLength;
	int   nNumBursts;
	int	  nAveNOPB;
	int	  nNumOpenings;
	float fAveTOT;
	float fAveBstDuration;
	int   nPrecedingICC;		//The position (in the unit of event) of the preceding inter-cluster interval
	float fPrecedingICC;	//The length (ms) of the preceding inter-cluster interval
	int   nMaxNOPB;
	int   nFirstOpening;
	int   nLastOpening;
	int   nFirstBurst;
	int   nLastBurst;
};

struct structClusterNodes{
	int nEvlID;
	int nBurstID;
	int nClusterID;
	double dTcb;
	double dTcc;
	vector<structClusterNode> clusterNodes;
};

struct structEvlDTFittingConstraintsNode{
	int nDimT;
	int nDimW;
	vector <int> nvT0; //this vector contain the index for free timeconstants;
	vector <int> nvTc; //this vector contain the index for frozen timeconstants;
	vector <int> nvW0; //this vector contain the index for free weights;
	vector <int> nvWc; //this vector contain the index for frozen weights;
	int nNumComponents;
	float fW0;
	float fWc;
};

struct structGammaFittingConstraintsNode{
	int nDimR;
	int nDimW;
	vector <int> nvR0; //this vector contain the index for free timeconstants;
	vector <int> nvRc; //this vector contain the index for frozen timeconstants;
	vector <int> nvW0; //this vector contain the index for free weights;
	vector <int> nvWc; //this vector contain the index for frozen weights;
	int nNumComponents;
	float fW0;
	float fWc;
};

struct structExponentialFittingConstraintsNode{
	int nDimL;
	int nDimW;
	vector <int> nvL0; //this vector contain the index for free timeconstants;
	vector <int> nvLc; //this vector contain the index for frozen timeconstants;
	vector <int> nvW0; //this vector contain the index for free weights;
	vector <int> nvWc; //this vector contain the index for frozen weights;
	int nNumComponents;
	float fW0;
	float fWc;
};

struct structGeometricFittingConstraintsNode{
	int nDimR;
	int nDimW;
	vector <int> nvR0; //this vector contain the index for free timeconstants;
	vector <int> nvRc; //this vector contain the index for frozen timeconstants;
	vector <int> nvW0; //this vector contain the index for free weights;
	vector <int> nvWc; //this vector contain the index for frozen weights;
	int nNumComponents;
	float fW0;
	float fWc;
};


struct structKineticStateNode{
	float fAmp;
	float fDev;
	int nLevel;
};


struct structHistNode{
	double dData;
	double dLeft;
	double dRight;
};

struct structHBHistParNode{
	double dLinMin;
	double dLinDelta;
	double dLogMin;
	double dLogMax;
	double dLogDelta;
};

struct structpClampResultNode
{
	int nNumLines;
	int nLineLength;
	char unit;
	vector <float> vfData;
};

struct structLTCNode  //Level Transition Criteria Node
{
	float fUTMin;
	float fUTMax;
	float fDTMin;//Downtransition threshold for local minima;
	float fDTMax;//Downtransition threshold for local maxima;
	float fDTDelta;
	float fUTDelta;
};


struct structEventNode
{
	short stLevel;
	int LevelStart;
	int LevelLength;
	float fAmpMean;
	float fAmpDev;
	float fAmpMax;
	float fAmpMin;
	bool bAnchor;
	short stState;
};

struct structEventListNode
{
	vector <structEventNode> vstructEventList;
	CString sFileName;
	int nNumChannels;
	int nNumLevels;
	float fTimePerPoint;
};

struct structOneDHistogramNode
{
	float fMin;
	float fInterval;
	vector <float> vfHist;
	vector <float> vfHist_Sum;
	float fTotalCounts;
	float fMean;
	float f5_thBin; //the 4th nonzero bin
};


struct structStateSummaryNode
{
	int nState;
	float fAmp;
	float fDev;
	int nLevel;
	int nNumEvents;
	double dTLength;
	double dMeanLength;
	double dRecordLength;
};

struct structBurstSemNode{
	structSemNode fSemAmp;
	structSemNode fSemMTo;
	structSemNode fSemNOPB;
	structSemNode fSemMBTo;
	structSemNode fSemToT;
};

struct structScaledQMNode{
	int i1;
	int j1;
	bool bForward;
	double dScale; // dScale=q(i0,j0)/q(i1,j1) q(i0,j0) is the element initialialized in the input file.
};

struct structQMModelNode{
	int i0;
	int j0;
	bool bFixf; //ture indicates that the forward rate constant Q(i0,j0) is to keep the fixed input value.
	bool bFixr; //ture indicates that the reverse rate constant Q(j0,i0) is to keep the fixed input value.
	int nNumScaledEf; // the number of other elements keeping canstant scales with the forward rate constant.
	int nNumScaledEr; // the number of other elements keeping canstant scales with the reverse rate constant.
	vector <structScaledQMNode> svScaledElements;
	double dV1;
	double dV2;
};

struct structQMElementNode{
	int i0;
	int j0;
	double dV1; //Q(i0,j0)
	double dV2; //Q(j0,i0)
};

struct structQMConstraintNode{
	int nType; //1 fixted value, 2 fixed ration with another element
	int i0;
	int j0;
	int i1;
	int j1;
	double dValue; //value of the fixed rate constant when nType==1, the value of Q(i0,j0)/Q(i1,j1) when nType==2
	double dScale; //value of Q(i0,j0)/Q(i1,j1) when nType==2
};

struct structFMCParentNode{
	int nParentIndex; //The index of the member in the list of F matrix constraints
	int nBirthIndex; //The index in the children list of the parent
	int nCommonStates; // 0 for F(i0,i0), 1 for F(i0,j0), 2 for F(i1,i1) and 3 for F(i1,j1)
};

struct structFMCChildNode{
	int nChildIndex; //The index of the member in the list of F matrix constraints
	int nCommonStates; // 0 for F(i0,i0), 1 for F(i0,j0), 2 for F(i1,i1) and 3 for F(i1,j1)
};

struct structFMConstraintNode{
	int nPIndex; //The pool index, a pool is a set of FMConstraints sharing common involved F matrix elements.
	int nCIndex; //The constraint index, as the input order.
	int nType; //1 fixted value, 2 fixed ration with another element
	int i0;
	int j0;
	int i1;
	int j1;
	double dValue; //value of the fixed rate constant when nType==1, the value of Q(i0,j0)/Q(i1,j1) when nType==2
	vector <int> vnInvolvedFMEs;  //stores the indexes (in the input list) of involved F matrix elements.
	int nSpecifiedI; //The index of the specified element. 0 indicate specifying i1,j1 as the dependent elements. 1 indicate 
		//specifying i1,i1 as the dependent element. 2 indicate specifying i0,j0 as the dependent elements.
	    //3 indicate specifying i0,i0 as the dependent elements.
	int nPCI; //The index of the constraint among the pool.
	vector <int> vnFamilyTree;  //List of the members from the first constraint in the pool
	structFMCParentNode structParent;
	vector <structFMCChildNode> vsChildren;
	vector <int> vnFreeFME;
};

struct structFMCPoolNode{
	int nPoolIndex;
	vector <int> vnMembers; //list of index of the F matrix constrain (FMC) indeces. 
};

struct structFMElementNode{
	int nConstraintIndex; //-1 if the element is an independent one, other wise is the index of the constrain by which it is specified as 
	                      // a dependent element.
	int i0;
	int j0;
	double dValue; //The value of the matrix element.
	int nCFMEIndex; //The index among constrained F matrix elements
	vector <int> vnRelatedConstraints; //List of all constraints in which the element is involved
};

struct structLevelPropertyNode{
	int nLevel;
	float fAmp;
	float fDev;
};

struct structDataSimulationNode{//input parameters for single-channel recording data simulations
	int nNumLevels;
	int nNumStates;
	vector <structQMModelNode> vsInputModel;
	vector <nVect> vvnStatesInLevels;
	vector <structLevelPropertyNode> vsLevelProperties;
	vector <dVect> vvdQMatrix;
	float fTimePerPoint;
	float fLength;
};

struct structBurstingTimeCourseOptionNode{//input parameters for single-channel recording data simulations
	float fBurstDelimiter,fClusterDelimiter,fBinWidth,fLatancy;
	float fMinCTo,fMaxCTo,fMinIBC,fMaxIBC,fMinBurstDuration,fMaxBurstDuration;
	int nBinSize,nMaxOpening,nMinOpening,nSortingIndex,nEICutoff,nBurstDelimiterIndex,nLatancy,nFilteringKey,nSubConductanceLevelI,nSubConductanceLevelF;
	vector<int> vnFilteringKeys;
	CString sFilteringKey;
	int nNumFilteringKeys;
	int nMinNOPC,nMaxNOPC;
	bool bSet;
};

struct structMeanSemNode{
	int nSize;
	double dMean;
	double dSem;
};

struct structIncidenceCriterionNode{
	int nRelatingIndex;//an incidence i will be included into the subset if the incidence i+nRelatingIndex match the 
		   //specified criterion.
	CString sCriterionID;//one of the string identification for criterion.
	int nCriterionID;//an integer ID generated by looking up String - int ID table.
	double dV1, dV2; //Only the first value will be used if the ID indicates as a single value criterion. 
				 //Also will be converted
	int nIncidenceType;//0 for event, 1 for burst, and 2 for cluster

};
struct structIncidenceSubsetCriteriaNode{
	int nIncidenceType; //0 for event, 1 for burst, and 2 for cluster
	int nApplyingIndex;
	int nOperationType; // 1 for AND, 2 for OR. Can be negative, in which case the subset will be converted to 
					   // to its complimentary subset before applying.
	vector <structIncidenceCriterionNode> vstructIncidenceCriteria;
};

struct structIncidenceSubsetNode{
	int nIncidenceType; //0 for event, 1 for burst, and 2 for cluster
	int nApplyingIndex; //a negative integer for preceding incidence, zero for self and a positive integer for the following incidence.
	int nOperationType; // 1 for AND, 2 for OR. Can be negative, in which case the subset will be converted to 
					   // to its complimentary subset before applying.
	vector <int> vnIndexes; //indexes of the incidences.
	structIncidenceSubsetCriteriaNode criteria;//the subset criteria used to construct the subset.
};

struct structIncidenceSubsetCollectionNode{
	int nEvlID;
	int nBurstID;
	int nClusterID;
	double dTcb,dTcc;//burst and cluster delimiters.
	vector<structIncidenceSubsetNode> vstructSubsets;
};

	/* 
	list of the criteria:
	level, dwellTimeRange(ms), conductanceRange(pA), latancyRangeT(ms), latancyRangeN(openings)
	IBCRange(ms), cLatancyRangeT(ms), cLatancyRangeT(bursts), BToRange(ms), NOPBRange.
	ICCRanges(ms), CToRanges(ms), NOPCRange(bursts).
	*/

#endif // !defined(AFX_EVLNODE_H__829A080D_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
