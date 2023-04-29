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
#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_MetaSoundClosestWaveSelector"

namespace Metasound
{
	// Vertex Names - define your node's inputs and outputs here
	namespace ClosestWaveSelector
	{
		// METASOUND_PARAM(InputAValue, "A", "Input value A.");
		// METASOUND_PARAM(InputBValue, "B", "Input value B.");
        METASOUND_PARAM(InputTrigger, "Input Trigger", "Input Trigger.");
        METASOUND_PARAM(InputWaveAssetArray, "WaveAssetArray", "Sound Waves Input");
        METASOUND_PARAM(InputTargetNoteNumber, "Note Number", "Note Number");
        METASOUND_PARAM(OutputTrigger, "Output", "Output Trigger");
		METASOUND_PARAM(OutputWaveAssetSample, "Sound Wave Output", "Sound Wave Output");
        METASOUND_PARAM(OutputPitchShiftNumber, "Pitch Shift", "Pitch Shift");
	}

	// Operator Class - defines the way your node is described, created and executed
	class FClosestWaveSelectorOperator : public TExecutableOperator<FClosestWaveSelectorOperator>
	{
		public:
            using FArrayWaveSampleReadRef = TDataReadReference<TArray<FWaveAsset>>;
		    using WaveSampleArrayType = TArray<FWaveAsset>;
			// Constructor
			FClosestWaveSelectorOperator(
				// const FFloatReadRef& InAValue,
				// const FFloatReadRef& InBValue,
                const FCreateOperatorParams& InParams,
                const FTriggerReadRef& InAValue,
                const FArrayWaveSampleReadRef& InWaveAssets,
                const FFloatReadRef& InNoteNumber):
				//   InputA(InAValue)
				// , InputB(InBValue)
                  InputTrigger(InAValue)
                , WaveAssets(InWaveAssets)
                , TargetNote(InNoteNumber)
                , OutputTrigger(FTriggerWriteRef::CreateNew(InParams.OperatorSettings))
                , WaveAssetSample(FWaveAssetWriteRef::CreateNew())
                , OutputPitchShift(FFloatWriteRef::CreateNew())
			{
			}

			// Helper function for constructing vertex interface
			static const FVertexInterface& DeclareVertexInterface()
			{
				using namespace ClosestWaveSelector;

				static const FVertexInterface Interface(
					FInputVertexInterface(
						// TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputAValue)),
						// TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputBValue)),
                        TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_TT(InputTrigger)),
                        TInputDataVertex<WaveSampleArrayType>(METASOUND_GET_PARAM_NAME_AND_TT(InputWaveAssetArray)),
                        TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputTargetNoteNumber))
					),
					FOutputVertexInterface(
                        TOutputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_TT(OutputTrigger)),
						TOutputDataVertex<FWaveAsset>(METASOUND_GET_PARAM_NAME_AND_TT(OutputWaveAssetSample)),
                        TOutputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_TT(OutputPitchShiftNumber))
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
						FNodeClassName { "UE", "ClosestWaveSelector Node", "Audio" }, 
						1, // Major Version
						0, // Minor Version
						METASOUND_LOCTEXT("ClosestWaveSelectorDisplayName", "ClosestWaveSelector Node"),
						METASOUND_LOCTEXT("ClosestWaveSelectorDesc", "Select the closest wave asset to the target note."),
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
				using namespace ClosestWaveSelector;

				FDataReferenceCollection InputDataReferences;

				// InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputAValue), InputA);
				// InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputBValue), InputB);
                InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputTrigger), InputTrigger);
                InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputWaveAssetArray), WaveAssets);
                InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputTargetNoteNumber), TargetNote);

				return InputDataReferences;
			}

			// Allows MetaSound graph to interact with your node's outputs
			virtual FDataReferenceCollection GetOutputs() const override
			{
				using namespace ClosestWaveSelector;

				FDataReferenceCollection OutputDataReferences;

                OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputTrigger), OutputTrigger);
				OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputWaveAssetSample), WaveAssetSample);
                OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputPitchShiftNumber), OutputPitchShift);

				return OutputDataReferences;
			}

			// Used to instantiate a new runtime instance of your node
			static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
			{
				using namespace ClosestWaveSelector;

				const Metasound::FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
				const Metasound::FInputVertexInterface& InputInterface = DeclareVertexInterface().GetInputInterface();

				// TDataReadReference<float> InputA = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputAValue), InParams.OperatorSettings);
				// TDataReadReference<float> InputB = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputBValue), InParams.OperatorSettings);
                TDataReadReference<FTrigger> InputTrigger = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FTrigger>(InputInterface, METASOUND_GET_PARAM_NAME(InputTrigger), InParams.OperatorSettings);
                TDataReadReference<WaveSampleArrayType> WaveAssets = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<WaveSampleArrayType>(InputInterface, METASOUND_GET_PARAM_NAME(InputWaveAssetArray), InParams.OperatorSettings);
				TDataReadReference<float> TargetNote = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputTargetNoteNumber), InParams.OperatorSettings);
                
                return MakeUnique<FClosestWaveSelectorOperator>(InParams, InputTrigger ,WaveAssets, TargetNote);
			}

			// Primary node functionality
			void Execute()
			{
                OutputTrigger->AdvanceBlock();

                InputTrigger->ExecuteBlock(
					[](int32, int32) 
					{
					},
					[this](int32 StartFrame, int32 EndFrame)
					{
                        int ArrayLength = WaveAssets->Num();
                        TArray<FWaveAsset> TempAssets = *WaveAssets;
                        FWaveAsset TempWave;
                        float TempShift;
                        float Minimum = 100;
                        for (int i = 0; i < ArrayLength; i++)
                        {
                            FSoundWaveProxyPtr WaveProxy = TempAssets[i].GetSoundWaveProxy();
                            if(WaveProxy.IsValid())
                            {
                                FWaveAsset Wave= TempAssets[i];
                                FString First = GetLastChars(Wave->GetFName().ToString(), 1);
                                FString Second = GetLastChars(Wave->GetFName().ToString(), 2);
                                float NoteDifference = *TargetNote - CovertToMIDINum(First, Second);
                                float Abs = NoteDifference < 0? NoteDifference*-1: NoteDifference;

                                if(Minimum > Abs)
                                {
                                    Minimum = Abs;
                                    TempShift = NoteDifference;
                                    TempWave = Wave;
                                }
                            }                                   
                        }
                        *WaveAssetSample = TempWave;
                        *OutputPitchShift = TempShift;
                        OutputTrigger->TriggerFrame(StartFrame);
                        //UE_LOG(LogTemp, Warning, TEXT("Excuting!"));
                    });

			}

	private:

		// Inputs
		// FFloatReadRef InputA;
        FTriggerReadRef InputTrigger;
		FFloatReadRef TargetNote;
        FArrayWaveSampleReadRef WaveAssets;

		// Outputs
        FTriggerWriteRef OutputTrigger;
		FFloatWriteRef OutputPitchShift;
        FWaveAssetWriteRef WaveAssetSample;

		FString GetLastChars(FString InString, int ReversedIndex)
		{
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

		float CovertToMIDINum(FString first, FString second)
		{
			//C3 is middle note
			//C-2 = 0
			//C-1 = 12
			//alphabet+12*(n+2)

			if(first.IsNumeric()&&!second.IsNumeric())
			{
				float degreeNum = (FCString::Atof(*first)+2)*12;
				float noteNum = AlphabetToNum(second);
				return degreeNum + noteNum;
			}
			else if(second.IsNumeric()&&!first.IsNumeric())
			{
				float degreeNum = (FCString::Atof(*second)+2)*12;
				float noteNum = AlphabetToNum(first);
				return degreeNum + noteNum;
			}
			else
				return -1;
			
		}
		float AlphabetToNum(FString note)
		{
			const TCHAR* chars = *note;
			switch (chars[0]) 
			{
				case 'C':
					return 0.0f;
					break;
				case 'D':
					return 2.0f;
					break;
				case 'E':
					return 4.0f;
					break;
				case 'F':
					return 5.0f;
					break;
				case 'G':
					return 7.0f;
					break;
				case 'A':
					return 9.0f;
					break;
				case 'B':
					return 11.0f;
					break;
				case 'c':
					return 0.0f;
					break;
				case 'd':
					return 2.0f;
					break;
				case 'e':
					return 4.0f;
					break;
				case 'f':
					return 5.0f;
					break;
				case 'g':
					return 7.0f;
					break;
				case 'a':
					return 9.0f;
					break;
				case 'b':
					return 11.0f;
					break;
				default: break;
			}
			return 0;
		}
	};

	// Node Class - Inheriting from FNodeFacade is recommended for nodes that have a static FVertexInterface
	class FClosestWaveSelector : public FNodeFacade
	{
		public:
			FClosestWaveSelector(const FNodeInitData& InitData)
				: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FClosestWaveSelectorOperator>())
			{
			}
	};

	// Register node
	METASOUND_REGISTER_NODE(FClosestWaveSelector);
}
#undef LOCTEXT_NAMESPACE