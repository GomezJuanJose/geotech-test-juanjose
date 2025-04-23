#pragma once

UENUM()
enum ETileStatus : int32
{
	FIRST_OPENED = -3,
	MINE = -2,
	NONE = -1,
	REVEALED = 0,
	ONE_MINE = 1
};