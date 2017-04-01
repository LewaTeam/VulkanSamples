/*
 * LunarGravity - gravityclock.hpp
 *
 * Copyright (C) 2017 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <time.h>

class GravityClock {
public:
    GravityClock() {
        m_paused = true;
    }

private:
    bool m_paused;
    double m_last_comp_time;
    double m_last_game_time;

    double GrabCurrentTime(void) {
        timespec current;
        clock_gettime(CLOCK_MONOTONIC, &current);
        return static_cast<double>(current.tv_sec) +
                    (static_cast<double>(current.tv_nsec) * 0.000001);
    }

public:

    void Start() {
        timespec current;
        clock_gettime(CLOCK_MONOTONIC, &current);
        m_last_comp_time = GrabCurrentTime();
    }

    void StartGameTime() {
        timespec current;
        clock_gettime(CLOCK_MONOTONIC, &current);
        m_last_game_time = GrabCurrentTime();
        m_paused = false;
    }

    void PauseGameTime() { m_paused = true; }

    void GetTimeDiffMS(float &comp_diff, float &game_diff) {
        timespec current;
        double cur_time;
        while (true) {
            clock_gettime(CLOCK_MONOTONIC, &current);
            cur_time = GrabCurrentTime();
            comp_diff = static_cast<float>(cur_time - m_last_comp_time);
            if (comp_diff < 1.0f) {
                continue;
            }
            m_last_comp_time = cur_time;
            if (!m_paused) {
                game_diff = static_cast<float>(cur_time - m_last_game_time);
                m_last_game_time = cur_time;
            }
            break;
        }
    }
};