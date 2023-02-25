#include "MetasoundExecutableOperator.h"     // TExecutableOperator class
#include "MetasoundPrimitives.h"             // ReadRef and WriteRef descriptions for bool, int32, float, and string
#include "MetasoundNodeRegistrationMacro.h"  // METASOUND_LOCTEXT and METASOUND_REGISTER_NODE macros
#include "MetasoundStandardNodesNames.h"     // StandardNodes namespace
#include "MetasoundFacade.h"				         // FNodeFacade class, eliminates the need for a fair amount of boilerplate code
#include "MetasoundParamHelper.h"            // METASOUND_PARAM and METASOUND_GET_PARAM family of macros
#include "MetasoundWave.h"
#include "UObject/UObjectBase.h"
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "MetasoundAssetBase.h"
#include "Subsystems/EngineSubsystem.h"
#include "Templates/Function.h"
#include "Sound/SoundWave.h"
#include "MetasoundUObjectRegistry.h"


// Required for ensuring the node is supported by all languages in engine. Must be unique per MetaSound.
#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_MetaSoundGetFilesName"

namespace Metasound
{
	// Vertex Names - define your node's inputs and outputs here
	namespace GetFilesName
	{
		// METASOUND_PARAM(InputAValue, "A", "Input value A.");
		// METASOUND_PARAM(InputBValue, "B", "Input value B.");
        METASOUND_PARAM(InputWaveAssetName, "Sample", "Sound Wave Input");
		METASOUND_PARAM(OutputStringName1, "Last Char of Wave Asset", "Last Char of Wave Asset.");
		METASOUND_PARAM(OutputStringName2, "Last Second Char of Wave Asset", "Last Second Char of Wave Asset.");
	}
    namespace StandardNodes
	{
		const FName Namespace = "UE";
		const FName AudioVariant = "Audio";
	}

	// Operator Class - defines the way your node is described, created and executed
	class FGetFilesNameOperator : public TExecutableOperator<FGetFilesNameOperator>
	{
		public:
			// Constructor
			FGetFilesNameOperator(
				// const FFloatReadRef& InAValue,
				// const FFloatReadRef& InBValue,
                const FWaveAssetReadRef& WaveAsset):
				//   InputA(InAValue)
				// , InputB(InBValue)
                WaveAsset(WaveAsset)
				, GetFilesNameOutput1(FStringWriteRef::CreateNew())
				, GetFilesNameOutput2(FStringWriteRef::CreateNew())
			{
			}

			// Helper function for constructing vertex interface
			static const FVertexInterface& DeclareVertexInterface()
			{
				using namespace GetFilesName;

				static const FVertexInterface Interface(
					FInputVertexInterface(
						// TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputAValue)),
						// TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputBValue)),
                        TInputDataVertexModel<FWaveAsset>(METASOUND_GET_PARAM_NAME_AND_TT(InputWaveAssetName))
					),
					FOutputVertexInterface(
						TOutputDataVertexModel<FString>(METASOUND_GET_PARAM_NAME_AND_TT(OutputStringName1)),
						TOutputDataVertexModel<FString>(METASOUND_GET_PARAM_NAME_AND_TT(OutputStringName2))
					)
				);

				return Interface;
			}

			// Retrieves necessary metadata about your node
			static const FNodeClassMetadata& GetNodeInfo()
			{
				auto CreateNodeClassMetadata = []() -> FNodeClassMetadata
				{
					FVertexInterface NodeInterface = DeclareVertexInterface();

					FNodeClassMetadata Metadata
					{
						FNodeClassName { StandardNodes::Namespace, "GetFilesName Node", StandardNodes::AudioVariant }, 
						1, // Major Version
						0, // Minor Version
						METASOUND_LOCTEXT("GetFilesNameDisplayName", "GetFilesName Node"),
						METASOUND_LOCTEXT("GetFilesNameDesc", "A simple node to demonstrate how to create new MetaSound nodes in C++. Adds two floats together"),
						PluginAuthor,
						PluginNodeMissingPrompt,
						NodeInterface,
						{ }, // Category Hierarchy 
						{ }, // Keywords for searching
						FNodeDisplayStyle{}
					};

					return Metadata;
				};

				static const FNodeClassMetadata Metadata = CreateNodeClassMetadata();
				return Metadata;
			}

			// Allows MetaSound graph to interact with your node's inputs
			virtual FDataReferenceCollection GetInputs() const override
			{
				using namespace GetFilesName;

				FDataReferenceCollection InputDataReferences;

				// InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputAValue), InputA);
				// InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputBValue), InputB);
                InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputWaveAssetName), WaveAsset);

				return InputDataReferences;
			}

			// Allows MetaSound graph to interact with your node's outputs
			virtual FDataReferenceCollection GetOutputs() const override
			{
				using namespace GetFilesName;

				FDataReferenceCollection OutputDataReferences;

				OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputStringName1), GetFilesNameOutput1);
				OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputStringName2), GetFilesNameOutput2);

				return OutputDataReferences;
			}

			// Used to instantiate a new runtime instance of your node
			static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
			{
				using namespace GetFilesName;

				const Metasound::FDataReferenceCollection& WaveAssetollection = InParams.InputDataReferences;
				const Metasound::FInputVertexInterface& InputInterface = DeclareVertexInterface().GetInputInterface();

				// TDataReadReference<float> InputA = WaveAssetollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputAValue), InParams.OperatorSettings);
				// TDataReadReference<float> InputB = WaveAssetollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputBValue), InParams.OperatorSettings);
                TDataReadReference<FWaveAsset> WaveAsset = WaveAssetollection.GetDataReadReferenceOrConstructWithVertexDefault<FWaveAsset>(InputInterface, METASOUND_GET_PARAM_NAME(InputWaveAssetName), InParams.OperatorSettings);
				return MakeUnique<FGetFilesNameOperator>(WaveAsset);
			}

			// Primary node functionality
			void Execute()
			{
				CurrentWaveAsset = *WaveAsset;
				FSoundWaveProxyPtr WaveProxy = CurrentWaveAsset.GetSoundWaveProxy();
				if(WaveProxy.IsValid())
				{
					*GetFilesNameOutput1 = GetLastChars(*CurrentWaveAsset->GetFName().ToString(), 1);
					*GetFilesNameOutput2 = GetLastChars(*CurrentWaveAsset->GetFName().ToString(), 2);
				}
			}

	private:

		// Inputs
		// FFloatReadRef InputA;
		// FFloatReadRef InputB;
        FWaveAssetReadRef WaveAsset;
		FWaveAsset CurrentWaveAsset;

		// Outputs
		FStringWriteRef GetFilesNameOutput1;
		FStringWriteRef GetFilesNameOutput2;

		FString GetLastChars(FString InString, int ReversedIndex)
		{
			const TCHAR* result = *InString;
			int lens = InString.Len();
			if(lens<2 || (ReversedIndex<1||ReversedIndex>2))
				return InString;
			else
			{	
				if(ReversedIndex == 1)
				{
					return InString.Mid(lens-1,1);
				}
				else
				{
					return InString.Mid(lens-2,1);
				}	
			}
		}
	};

	// Node Class - Inheriting from FNodeFacade is recommended for nodes that have a static FVertexInterface
	class FGetFilesName : public FNodeFacade
	{
		public:
			FGetFilesName(const FNodeInitData& InitData)
				: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FGetFilesNameOperator>())
			{
			}
	};

	// Register node
	METASOUND_REGISTER_NODE(FGetFilesName);
}
#undef LOCTEXT_NAMESPACE