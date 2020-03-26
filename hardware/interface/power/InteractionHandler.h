/*
 * Copyright (C) 2017 The Android Open Source Project
 * Copyright (C) 2018 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef INTERACTIONHANDLER_H
#define INTERACTIONHANDLER_H

#include <condition_variable>
#include <mutex>
#include <thread>

enum interaction_state {
    INTERACTION_STATE_UNINITIALIZED,
    INTERACTION_STATE_IDLE,
    INTERACTION_STATE_INTERACTION,
};

struct InteractionHandler {
    InteractionHandler();
    ~InteractionHandler();
    bool Init();
    void Exit();
    void Acquire(int32_t duration);

 private:
    bool openEpic();
    void Release();
    void Routine();

    void PerfLock();
    void PerfRel();

    long long CalcTimespecDiffMs(struct timespec start, struct timespec end);

    enum interaction_state mState;

    int32_t mMinDurationMs;
    int32_t mMaxDurationMs;
    int32_t mDurationMs;

    struct timespec mLastTimespec;

    bool mHandled;

    int64_t (*_epic_alloc_request)(void);
    int64_t (*_epic_free_request)(void);
    int64_t (*_epic_acquire)(void);
    int64_t (*_epic_release)(void);
    bool mEpicAvailable;

    std::unique_ptr<std::thread> mThread;
    std::mutex mLock;
    std::condition_variable mCond;
};

#endif //INTERACTIONHANDLER_H
