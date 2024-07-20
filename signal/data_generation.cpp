#include "data_generation.h"
#include <qmath.h>

void SineWaveGenerator::set_generator_parameters( double& in_amplitude, double& in_frequency, double& in_phase, double& in_sample_rate, double& in_duration ) {
    parameters_[ "amplitude" ]   = in_amplitude;
    parameters_[ "frequency" ]   = in_frequency;
    parameters_[ "phase" ]       = in_phase;
    parameters_[ "sample_rate" ] = in_sample_rate;
    parameters_[ "duration" ]    = in_duration;
}

void SineWaveGenerator::generate_data( std::vector< double >& out_sine_wave_data ) {
    is_generator_completed_ = false;

    double amplitude   = parameters_[ "amplitude" ];
    double frequency   = parameters_[ "frequency" ];
    double phase       = parameters_[ "phase" ];
    double sample_rate = parameters_[ "sample_rate" ];
    double duration    = parameters_[ "duration" ];

    double dt         = 1.0 / sample_rate;
    int    num_points = static_cast< int >( duration / dt );
    out_sine_wave_data.reserve( num_points );

    for ( double t = 0; t < duration; t += dt ) {
        double y = amplitude * qSin( 2 * M_PI * frequency * t + phase );
        out_sine_wave_data.push_back( y );
    }

    is_generator_completed_ = true;
}

void SquareWaveGenerator::set_generator_parameters( double& in_amplitude, double& in_frequency, double& in_phase, double& in_sample_rate, double& in_duration ) {
    parameters_[ "amplitude" ]   = in_amplitude;
    parameters_[ "frequency" ]   = in_frequency;
    parameters_[ "phase" ]       = in_phase;
    parameters_[ "sample_rate" ] = in_sample_rate;
    parameters_[ "duration" ]    = in_duration;
}

void SquareWaveGenerator::generate_data( std::vector< double >& out_square_wave_data ) {
    is_generator_completed_ = false;

    double amplitude   = parameters_[ "amplitude" ];
    double frequency   = parameters_[ "frequency" ];
    double phase       = parameters_[ "phase" ];
    double sample_rate = parameters_[ "sample_rate" ];
    double duration    = parameters_[ "duration" ];

    double dt         = 1.0 / sample_rate;
    int    num_points = static_cast< int >( duration / dt );
    out_square_wave_data.reserve( num_points );
    for ( double t = 0; t < duration; t += dt ) {
        double y = amplitude * qSin( 2 * M_PI * frequency * t + phase );
        if ( y > 0 ) {
            y = amplitude;
        }
        else {
            y = -amplitude;
        }
        out_square_wave_data.push_back( y );
    }

    is_generator_completed_ = true;
}

void TrapezoidWaveGenerator::set_generator_parameters( double in_amplitude, double in_frequency, double in_phase, double in_sample_rate, double in_duration, double in_rise_time, double in_high_time,
                                                       double in_fall_time ) {
    parameters_[ "amplitude" ]   = in_amplitude;
    parameters_[ "frequency" ]   = in_frequency;
    parameters_[ "phase" ]       = in_phase;
    parameters_[ "sample_rate" ] = in_sample_rate;
    parameters_[ "duration" ]    = in_duration;
    parameters_[ "rise_time" ]   = in_rise_time;
    parameters_[ "high_time" ]   = in_high_time;
    parameters_[ "fall_time" ]   = in_fall_time;
}

void TrapezoidWaveGenerator::generate_data( std::vector< double >& out_trapezoid_wave_data ) {
    is_generator_completed_ = false;

    double amplitude   = parameters_[ "amplitude" ];
    double frequency   = parameters_[ "frequency" ];
    double phase       = parameters_[ "phase" ];
    double sample_rate = parameters_[ "sample_rate" ];
    double duration    = parameters_[ "duration" ];
    double rise_time   = parameters_[ "rise_time" ];
    double high_time   = parameters_[ "high_time" ];
    double fall_time   = parameters_[ "fall_time" ];

    double dt = 1.0 / sample_rate;
    for ( double t = 0; t < duration; t += dt ) {
        double y = 0;
        if ( t < rise_time ) {
            y = amplitude / rise_time * t;
        }
        else if ( t < rise_time + high_time ) {
            y = amplitude;
        }
        else if ( t < rise_time + high_time + fall_time ) {
            y = -amplitude / fall_time * ( t - rise_time - high_time ) + amplitude;
        }
        out_trapezoid_wave_data.push_back( y );
    }

    is_generator_completed_ = true;
}
