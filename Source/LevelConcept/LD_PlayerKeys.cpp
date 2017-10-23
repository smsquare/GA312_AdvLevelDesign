// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_PlayerKeys.h"

FPlayerKeys::FPlayerKeys() {
	NumOfCyanKeys = 0;
	NumOfPurpleKeys = 0;
	NumOfYellowKeys = 0;
}

FPlayerKeys::~FPlayerKeys() {}

int FPlayerKeys::GetNumOfCyanKeys() const {
	return NumOfCyanKeys;
}
int FPlayerKeys::GetNumOfPurpleKeys() const {
	return NumOfPurpleKeys;
}
int FPlayerKeys::GetNumOfYellowKeys() const {
	return NumOfYellowKeys;
}