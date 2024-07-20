/**
 * @file data_generation.h
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#ifndef DATA_GENERATION_H
#define DATA_GENERATION_H

#include <map>
#include <string>
#include <vector>

class DataGenerator {
public:
    void set_generator_parameters();
    void generate_data();

public:
    bool is_generator_completed_ = false;

protected:
    std::map< std::string, double > parameters_;
};

/**
 * @brief Discrete sinusoidal signal generator
 *
 */
class SineWaveGenerator : public DataGenerator {
public:
    /**
     * @brief Set the required parameters for generating discrete sinusoidal
     * signals
     *
     * @param in_amplitude
     * @param in_frequency
     * @param in_phase
     * @param in_sample_rate
     * @param in_duration
     */
    void set_generator_parameters( double& in_amplitude, double& in_frequency, double& in_phase, double& in_sample_rate, double& in_duration );

    /**
     * @brief Generate discrete sinusoidal signal data
     *
     * @param out_sine_wave_data
     */
    void generate_data( std::vector< double >& out_sine_wave_data );
};

/**
 * @brief Discrete Square Wave Signal Generator
 *
 */
class SquareWaveGenerator : public DataGenerator {
public:
    /**
     * @brief Set the required parameters for generating discrete square wave
     * signals
     *
     * @param in_amplitude
     * @param in_frequency
     * @param in_phase
     * @param in_sample_rate
     * @param in_duration
     */
    void set_generator_parameters( double& in_amplitude, double& in_frequency, double& in_phase, double& in_sample_rate, double& in_duration );

    /**
     * @brief Generate discrete square wave signal data
     *
     * @param out_square_wave_data
     */
    void generate_data( std::vector< double >& out_square_wave_data );
};

class TrapezoidWaveGenerator : public DataGenerator {
public:
    /**
     * @brief Set the necessary parameters for the discrete ladder signal
     * generator
     *
     * @param amplitude
     * @param frequency
     * @param phase
     * @param sample_rate
     * @param duration
     * @param rise_time
     * @param high_time
     * @param fall_time
     */
    void set_generator_parameters( double in_amplitude, double in_frequency, double in_phase, double in_sample_rate, double in_duration, double in_rise_time, double in_high_time,
                                   double in_fall_time );

    /**
     * @brief Generate discrete trapezoidal signal data
     *
     * @param out_trapezoid_wave_data
     */
    void generate_data( std::vector< double >& out_trapezoid_wave_data );
};

#endif  // DATA_GENERATION_H
