/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common/OboeDebug.h"
#include "OboeStreamCallbackProxy.h"

#include "synth/IncludeMeOnce.h"

bool OboeStreamCallbackProxy::mCallbackReturnStop = false;

int64_t OboeStreamCallbackProxy::getNanoseconds(clockid_t clockId) {
    struct timespec time;
    int result = clock_gettime(clockId, &time);
    if (result < 0) {
        return result;
    }
    return (time.tv_sec * 1e9) + time.tv_nsec;
}

oboe::DataCallbackResult OboeStreamCallbackProxy::onAudioReady(
        oboe::AudioStream *audioStream,
        void *audioData,
        int numFrames) {
    oboe::DataCallbackResult callbackResult = oboe::DataCallbackResult::Stop;
    int64_t startTimeNanos = getNanoseconds();

    if (mCpuAffinityMask != mPreviousMask) {
        uint32_t mask = mCpuAffinityMask;
        applyCpuAffinityMask(mask);
        mPreviousMask = mask;
    }

    mCallbackCount++;
    mFramesPerCallback = numFrames;

    if (mCallbackReturnStop) {
        return oboe::DataCallbackResult::Stop;
    }

    if (mCallback != nullptr) {
        callbackResult = mCallback->onAudioReady(audioStream, audioData, numFrames);
    }

    mSynthWorkload.onCallback(mNumWorkloadVoices);
    if (mNumWorkloadVoices > 0) {
        float *buffer = (audioStream->getChannelCount() == 2 && mHearWorkload)
                        ? static_cast<float *>(audioData) : nullptr;
        mSynthWorkload.renderStereo(buffer, numFrames);
    }

    int64_t currentTimeNanos = getNanoseconds();
    double calculationTime = (double)(currentTimeNanos - startTimeNanos);
    double inverseRealTime = audioStream->getSampleRate() / (1.0e9 * numFrames);
    double currentCpuLoad = calculationTime * inverseRealTime; // avoid a divide
    mCpuLoad = (mCpuLoad * 0.95) + (currentCpuLoad * 0.05); // simple low pass filter

    if (mPreviousCallbackTimeNs != 0) {
        mStatistics.add((currentTimeNanos - mPreviousCallbackTimeNs) * kNsToMsScaler);
    }
    mPreviousCallbackTimeNs = currentTimeNanos;

    return callbackResult;
}
