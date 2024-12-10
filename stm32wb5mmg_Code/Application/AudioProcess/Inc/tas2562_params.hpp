#ifndef __TAS2562_PARAMS_HPP_
#define __TAS2562_PARAMS_HPP_

constexpr uint8_t PAGE{0x00U};
constexpr uint8_t BOOK{0x7FU};
constexpr uint8_t SW_RESET{0x01U};
constexpr uint8_t PWR_CTL{0x02U};
constexpr uint8_t PB_CFG1{0x03U};
constexpr uint8_t TDM_CFG0{0x06U};
constexpr uint8_t TDM_CFG1{0x07U};
constexpr uint8_t TDM_CFG2{0x08U};
constexpr uint8_t TDM_CFG3{0x09U};
constexpr uint8_t TDM_CFG4{0x0AU};
constexpr uint8_t TDM_CFG5{0x0BU};
constexpr uint8_t TDM_CFG6{0x0CU};
constexpr uint8_t MISC_CFG1{0x04U};
constexpr uint8_t MISC_CFG3{0x32U};
constexpr uint8_t BOOST_CFG2{0x34U};
constexpr uint8_t BOOST_CFG4{0x40U};
constexpr uint8_t INT_MASK0{0x1AU};
constexpr uint8_t INT_LTCH0{0x24U};

constexpr uint8_t PAGE_SET0{0x00U};     //! Page - 0
constexpr uint8_t BOOK_SET{0x00U};      //! Book - 0
constexpr uint8_t SWRE_SET{0x01U};      //! Reset
constexpr uint8_t PB_CFG0_SET{0x01U};   //! 8.5dB Gain
constexpr uint8_t TDM_CFG0_SET{0x02U};  //! 16 KHz, Auto TDM on, Frame Start Low to High //0X00U FOR 8KHZ
constexpr uint8_t TDM_CFG1_SET{0x03U};  //! Offset 1, Raising edge of SBCLK
constexpr uint8_t TDM_CFG2_SET{0x30U};  //! TDM slot by address,Word=16 bit, (L+R)/2
constexpr uint8_t TDM_CFG3_SET{0x00U};  //! Right Ch = TDM slot 0, Left Ch = TDM Slot 0
constexpr uint8_t TDM_CFG4_SET{0x13U};  //! TX bus keeper,Hi-Z, Offset 1, TX on Falling edge
constexpr uint8_t TDM_CFG5_SET{0x42U};  //! TDM TX voltage sense transmit enable with slot 2
constexpr uint8_t TDM_CFG6_SET{0x40U};  //! TDM TX current sense transmit enable with slot 0
constexpr uint8_t MISC_CFG1_SET{0x26U}; //! If overcurrent error occurs it retry in 1.5Sec
constexpr uint8_t MISC_IRQ_SET{0x80U};  //!	Set the IRQZ pin is low.
constexpr uint8_t PWR_CTL_SET{0x0DU};   //!	Mute the device.
constexpr uint8_t PWR_CTL_RESET{0x0CU}; //! Active the device.
constexpr uint8_t INT_MASK0_SET{0x00U}; //! Don't mask
constexpr uint8_t BOOST_CFG2_SET{0x0DU};
constexpr uint8_t BOOST_CFG4_SET{0x37U}; //! Boost peak current limit set to 1.1A

#endif /* __TAS2562_PARAMS_HPP_ */