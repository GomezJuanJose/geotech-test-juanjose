#pragma once

UENUM()
enum class ETileStatus : uint8
{
	NONE = 0,
	REVEALED = 1,
	FIRST_OPEN = 2,
	MINE = 3
};