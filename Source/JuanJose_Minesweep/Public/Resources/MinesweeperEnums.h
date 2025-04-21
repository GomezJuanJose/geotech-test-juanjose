#pragma once

UENUM()
enum ETileStatus : int32
{
	MINE = -2,
	NONE = -1,
	REVEALED = 0,
	ONE_MINE = 1
};