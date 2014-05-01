#include "shares.h"




class state_manager {
  
  
protected:
  
  const uint8_t NUM_ADCS;		/// number of adcs
  
  uint8_t * adc;			/// array of adc states
  
  const uint8_t NUM_MOTORS;		/// number of motors
  
  uint8_t * motor;			/// array of motor states
  
  int8_t * motor_power;			/// array of motor power commands
  
  uint8_t * motor_brake;		/// array of motor brake commands
  
  
public:
  
  state_manager();
  
  adc_state_enum adc_state( uint8_t, adc_state_enum );
  
  adc_state_enum adc_state( uint8_t );
  
  uint16_t adc_value( uint8_t );
  
  motor_state_enum motor_state( uint8_t, motor_state_enum );
  
  motor_state_enum motor_state( uint8_t );
  
  int16_t motor_power( uint8_t, int16_t );
  
  int16_t motor_power( uint16_t );
  
  uint16_t motor_brake( uint8_t, uint16_t );
  
  uint16_t motor_brake( uint8_t );
  
  