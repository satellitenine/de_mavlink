#ifndef VEHICLE_H_
#define VEHICLE_H_

#include <map>

#include <common/mavlink.h>
#include <ardupilotmega/ardupilotmega.h>

#include "mavlink_helper.h"

namespace mavlinksdk
{
    // 3 seconds
    #define HEART_BEAT_TIMEOUT 3000000l
    
 
    struct Time_Stamps
    {
        #define TIME_STAMP_MSG_LEN 1024

        Time_Stamps()
        {
            reset_timestamps();
        }

        uint64_t message_id[TIME_STAMP_MSG_LEN];
        
        void reset_timestamps()
        {
            for (int i=0; i< TIME_STAMP_MSG_LEN; ++i)
            {
                message_id[i] =0;
            }
        }

        #undef TIME_STAMP_MSG_LEN

    };


    



    class CCallBack_Vehicle
    {
        public:

        virtual void OnBoardRestarted ()                                                                                                {};
        virtual void OnHeartBeat_First (const mavlink_heartbeat_t& heartbeat)                                                           {};
        virtual void OnHeartBeat_Resumed (const mavlink_heartbeat_t& heartbeat)                                                         {};
        virtual void OnArmed (const bool& armed)                                                                                        {};
        virtual void OnFlying (const bool& isFlying)                                                                                    {};
        virtual void OnACK (const int& acknowledged_cmd, const int& result, const std::string& result_msg)                              {};
        virtual void OnStatusText (const std::uint8_t& severity, const std::string& status)                                             {};
        virtual void OnModeChanges(const int& custom_mode, const int& firmware_type)                                                    {};
        virtual void OnHomePositionUpdated(const mavlink_home_position_t& home_position)                                                {};
        virtual void OnServoOutputRaw(const mavlink_servo_output_raw_t& servo_output_raw)                                               {};
    };

    class CVehicle
    {
        
        public:
            //https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
            static CVehicle& getInstance()
            {
                static CVehicle instance;

                return instance;
            }

            CVehicle(CVehicle const&)               = delete;
            void operator=(CVehicle const&)         = delete;

        
            // Note: Scott Meyers mentions in his Effective Modern
            //       C++ book, that deleted functions should generally
            //       be public as it results in better error messages
            //       due to the compilers behavior to check accessibility
            //       before deleted status

        private:

            CVehicle();

        public:
            
            ~CVehicle ()
            {

            }

        public:

            void set_callback_vehicle (mavlinksdk::CCallBack_Vehicle* callback_vehicle);
            void parseMessage (const mavlink_message_t& mavlink_message);


        protected:

            void handle_heart_beat       (const mavlink_heartbeat_t& heartbeat);
            void handle_cmd_ack          (const mavlink_command_ack_t& command_ack);
            void handle_status_text      (const mavlink_statustext_t& status_text);
            void handle_home_position    (const mavlink_home_position_t& home_position);
            void handle_param_ext_value  (const mavlink_param_ext_value_t& param_message);
            void handle_param_value      (const mavlink_param_value_t& param_message);
            void handle_rc_channels_raw  (const mavlink_rc_channels_t& rc_channels);
            void handle_servo_output_raw (const mavlink_servo_output_raw_t& servo_output_raw);
            void handle_system_time      (const mavlink_system_time_t& system_time);
            
        // Vechile Methods
        public:
            const bool isFCBConnected() const;

            const mavlinksdk::FIRMWARE_TYPE getFirmwareType()
            {
                return m_firmware_type;
            }

            const bool isArmed()
            {
                return m_armed;
            }
            
            const bool isFlying() const
            {
                return m_is_flying;
            } 

            const mavlink_heartbeat_t& getMsgHeartBeat () const
            {
                return m_heartbeat;
            }

            const mavlink_sys_status_t& getMsgSysStatus () const
            {
                return m_sys_status;
            }

            const mavlink_battery_status_t& getMsgBatteryStatus () const
            {
                return m_battery_status;
            }

            const mavlink_radio_status_t& getMsgRadioStatus () const
            {
                return m_radio_status;
            }

            const mavlink_local_position_ned_t& getMsgLocalPositionNED () const
            {
                return m_local_position_ned;
            }

            const mavlink_global_position_int_t& getMsgGlobalPositionInt () const
            {
                return m_global_position_int;
            }

            const mavlink_position_target_local_ned_t& getMsgTargetPositionLocalNED () const
            {
                return m_position_target_local_ned;
            }

            const mavlink_position_target_global_int_t& getMsgTargetPositionGlobalInt () const
            {
                return m_position_target_global_int;
            }

            const mavlink_gps_raw_int_t& getMSGGPSRaw () const
            {
                return m_gps_raw_int;
            }

            const mavlink_attitude_t& getMsgAttitude () const
            {
                return m_attitude;
            }

            const mavlink_home_position_t& getMsgHomePosition () const
            {
                return m_home_position;
            }

            const mavlink_nav_controller_output_t& getMsgNavController() const
            {
                return m_nav_controller;
            }

            const mavlink_rc_channels_t& getRCChannels () const
            {
                return m_rc_channels;
            }

            const mavlink_system_time_t& getSystemTime() const 
            {  
                return m_system_time;
            }

            const std::string& getLastStatusText () const
            {
                return m_status_text;
            }

        // Class Members
        protected:
            mavlinksdk::CCallBack_Vehicle* m_callback_vehicle;
            bool m_heart_beat_first_recieved = false;


        // Vehicle Attributes
        protected:
            // Heartbeat
            mavlink_heartbeat_t m_heartbeat;

            // System Status
            mavlink_sys_status_t m_sys_status;

            // Battery Status
            /**
             * @brief 
             *  voltage in x1000 array of 10
             *  current in x100
             *  temprature in x 1000
             */
            mavlink_battery_status_t m_battery_status;

            // Radio Status
            mavlink_radio_status_t m_radio_status;

            // Local Position
            mavlink_local_position_ned_t m_local_position_ned;

            // Global Position
            mavlink_global_position_int_t m_global_position_int;

            // Local Position Target
            mavlink_position_target_local_ned_t m_position_target_local_ned;

            // Global Position Target
            mavlink_position_target_global_int_t m_position_target_global_int;

            // GPS Raw
            mavlink_gps_raw_int_t m_gps_raw_int;

            // HiRes IMU
            mavlink_highres_imu_t m_highres_imu;

            // Attitude
            mavlink_attitude_t m_attitude;

            // System Parameters?

            // Home Position
            mavlink_home_position_t m_home_position;

            // Desired (pitch, roll, yaw, wp_dist, alt_error)
            mavlink_nav_controller_output_t m_nav_controller;

            // RCChannels
            mavlink_rc_channels_t   m_rc_channels;

            //Servo Output Raw
            mavlink_servo_output_raw_t   m_servo_output_raw;
            
            // System Time
            mavlink_system_time_t  m_system_time;

            // Status Text
            std::string m_status_text;
            std::uint8_t m_status_severity =0;

            // Time Stamps
            Time_Stamps time_stamps;

            // Vehicle is armed
            bool m_armed     = false;
            // Flying or Diving => Armed Ready to take-off. It is not necessary physically flying
            bool m_is_flying = false;
            // Firmware Type
            mavlinksdk::FIRMWARE_TYPE m_firmware_type = FIRMWARE_TYPE_UNKNOWN;

    };
}


#endif // VEHICLE_H_
