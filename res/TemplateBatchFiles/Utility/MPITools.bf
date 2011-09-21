//------------------------------------------------------------------------------------------
function runAnalysisOnManyFiles (pathList, analysisName, callbackInput, callbackOutput)
{
	assert (MPI_NODE_COUNT > 1, "This analysis requires an MPI environment");
	_MPI_NODE_STATUS = {MPI_NODE_COUNT-1,1}["-1"]; 

	for (_fileLine = 0; _fileLine < Columns (pathList); _fileLine += 1)
	{
		_analysisOptions = Eval("`callbackInput` (_fileLine, pathList[_fileLine])");
		_SendAJob   (_fileLine, analysisName, _analysisOptions, callbackOutput);
	}
	
	_fileLine = +(_MPI_NODE_STATUS["_MATRIX_ELEMENT_VALUE_>=0"]);
	while (_fileLine >= 0)
	{
		_ReceiveAJob (callbackOutput);
	}

	return 0;
}

//------------------------------------------------------------------------------------------
function _SendAnMPIJob (jobNumber, fileID, options, callbackOutput)
{
	for (_mpiNode = 0; _mpiNode < MPI_NODE_COUNT-1; _mpiNode += 1)
	{
		if (MPI_NODE_STATUS[_mpiNode][0] == 0)
		{
			break;
		}
	}
	if (_mpiNode == MPI_NODE_COUNT-1)
	{
		mpiNode = ReceiveAJob (callbackOutput);
	}
	
	MPI_NODE_STATUS[mpiNode] = jobNumber;
	MPISend (mpiNode+1,"LoadFunctionLibrary (`fileID`," + options + ")");
	return 0;
}

//------------------------------------------------------------------------------------------
function _ReceiveAnMPIJob (callbackOutput)
{
	MPIReceive (-1,fromNode,result);
	fromNode += (-1);
	
	doneID   = MPI_NODE_STATUS[fromNode]-1;
	MPI_NODE_STATUS[fromNode] = -1;
	
	returnAVL = Eval(result);
	
	if (Abs(callbackOutput))
	{
		ExecuteCommands ("`callbackOutput`(doneID,returnAVL,pathList[doneID])");
	}
	
	return fromNode;
}