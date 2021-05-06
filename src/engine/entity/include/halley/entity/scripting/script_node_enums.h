#pragma once

#include <cstdint>

namespace Halley {
    enum class ScriptNodeExecutionState {
		Done,
		Executing,
		Restart,
		Terminate
	};

	enum class ScriptNodeClassification {
		Terminator, // As in start/end, not as in Arnie
		FlowControl,
		Variable,
		Action
	};

	enum class ScriptNodeElementType : uint8_t {
		Undefined,
		Node,
		FlowPin,
		DataPin,
		TargetPin
	};

	enum class ScriptNodePinDirection : uint8_t {
		Input,
		Output
	};

	enum class ScriptPinSide : uint8_t {
		Undefined,
		Left,
		Right,
		Top,
		Bottom
	};

	struct ScriptNodePinType {
		ScriptNodeElementType type = ScriptNodeElementType::Undefined;
		ScriptNodePinDirection direction = ScriptNodePinDirection::Input;

		bool operator==(const ScriptNodePinType& other) const
		{
			return type == other.type && direction == other.direction;
		}
		bool operator!=(const ScriptNodePinType& other) const
		{
			return type != other.type || direction != other.direction;
		}

		ScriptPinSide getSide() const
		{
			switch (type) {
			case ScriptNodeElementType::DataPin:
			case ScriptNodeElementType::FlowPin:
				return direction == ScriptNodePinDirection::Input ? ScriptPinSide::Left : ScriptPinSide::Right;
			case ScriptNodeElementType::TargetPin:
				return ScriptPinSide::Bottom;
			}
			return ScriptPinSide::Undefined;
		}
	};
}