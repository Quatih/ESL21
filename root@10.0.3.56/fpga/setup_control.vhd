--
-- @file setup_control.vhd
-- @brief Toplevel file template file which can be used as a reference for implementing gpmc communication.
-- @author Jan Jaap Kempenaar, University of Twente 2014
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity setup_control is
  generic(
    DATA_WIDTH           : integer := 16;
    GPMC_ADDR_WIDTH_HIGH : integer := 10;
    GPMC_ADDR_WIDTH_LOW  : integer := 1;
    -- RAM_SIZE should be a power of 2
    RAM_SIZE             : integer := 32
  );
  port (
    CLOCK_50      : in    std_logic;

    -- GPMC side
    GPMC_DATA     : inout std_logic_vector(DATA_WIDTH - 1 downto 0);
    GPMC_ADDR     : in    std_logic_vector(GPMC_ADDR_WIDTH_HIGH downto GPMC_ADDR_WIDTH_LOW);
    GPMC_nPWE     : in    std_logic;
    GPMC_nOE      : in    std_logic;
    GPMC_FPGA_IRQ : in    std_logic;
    GPMC_nCS6     : in    std_logic;
    GPMC_CLK      : in    std_logic;

    -- FPGA side
    -- Output headers
    F_IN          : in    std_logic_vector(15 downto 0);
    F_OUT         : out   std_logic_vector(15 downto 0);
    
    -- Pulse width modulators
    PWM1A         : out   std_logic;
    PWM1B         : out   std_logic;
    PWM1C         : out   std_logic;
    
    PWM2A         : out   std_logic;
    PWM2B         : out   std_logic;
    PWM2C         : out   std_logic;

    -- Encoders
    ENC1A         : in    std_logic;
    ENC1B         : in    std_logic;
    ENC1I         : in    std_logic;
    
    ENC2A         : in    std_logic;
    ENC2B         : in    std_logic;
    ENC2I         : in    std_logic
  );
end setup_control;


architecture structure of setup_control is
  -- GPMC controller component for FPGA
  component ramstix_gpmc_driver is
    generic(
      DATA_WIDTH           : integer := 16;
      GPMC_ADDR_WIDTH_HIGH : integer := 10;
      GPMC_ADDR_WIDTH_LOW  : integer := 1;
      RAM_SIZE             : integer := 32
    );
    port(
      clk           : in    std_logic;

      -- Data from FPGA to Overo Fire (input for GPMC driver)
      reg0_in       : in    std_logic_vector(DATA_WIDTH - 1 downto 0);
      reg1_in       : in    std_logic_vector(DATA_WIDTH - 1 downto 0);
      reg2_in       : in    std_logic_vector(DATA_WIDTH - 1 downto 0);
      reg3_in       : in    std_logic_vector(DATA_WIDTH - 1 downto 0);

      -- Data from Overo Fire to FPGA (output of GPMC driver)
      reg4_out      : out   std_logic_vector(DATA_WIDTH - 1 downto 0);
      reg5_out      : out   std_logic_vector(DATA_WIDTH - 1 downto 0);
      reg6_out      : out   std_logic_vector(DATA_WIDTH - 1 downto 0);
      reg7_out      : out   std_logic_vector(DATA_WIDTH - 1 downto 0);

      GPMC_DATA     : inout std_logic_vector(DATA_WIDTH - 1 downto 0);
      GPMC_ADDR     : in    std_logic_vector(GPMC_ADDR_WIDTH_HIGH downto GPMC_ADDR_WIDTH_LOW);
      GPMC_nPWE     : in    std_logic;
      GPMC_nOE      : in    std_logic;
      GPMC_FPGA_IRQ : in    std_logic;
      GPMC_nCS6     : in    std_logic;
      GPMC_CLK      : in    std_logic
    );

  end component;

  -- INSERT YOUR COMPONENTS HERE
    
  component Motor_Controller is
    generic (wl : natural := 32;
	    period : natural := 2500);
    port ( 
	    clk    : in std_logic;		--System clock, 50 MHz
	    reset  : in std_logic;		
	    enable : in std_logic;
	
	    --Outputs
	    INA		: out std_logic;	--INA input of the H-Bridge.
	    INB		: out std_logic;	--INB input of the H-bridge.
	    C		  : out std_logic;	--PWM input of the H-Bridge.
	    GPIO_0		: in std_logic_vector(wl -1 downto 0)
	);
  end component;


  component QuadratureDecoder is
   generic (wl : natural := 32);

   port ( 
	  clk   : in std_logic;
	  reset  : in std_logic;
	  enable : in std_logic;
	  GPIO_0		: inout std_logic_vector(wl -1 downto 0);
	  ENC_A : in std_logic;
	  ENC_B : in std_logic

	  );
  end component;

  -- Define signals to connect the component to the gpmc_driver
  signal msb_buffer_out   : std_logic_vector(DATA_WIDTH - 1 downto 0);
  signal lsb_buffer_out   : std_logic_vector(DATA_WIDTH - 1 downto 0);
  signal msb_buffer_in    : std_logic_vector(DATA_WIDTH - 1 downto 0);
  signal lsb_buffer_in    : std_logic_vector(DATA_WIDTH - 1 downto 0);
  signal enable : std_logic := '1';
  signal reset  : std_logic := '0';
  signal MC1_in : std_logic_vector(31 downto 0);
  signal MC2_in : std_logic_vector(31 downto 0);
  signal QD1_out : std_logic_vector(31 downto 0);
  signal QD2_out : std_logic_vector(31 downto 0);
  
begin
  -- Map GPMC controller to I/O.
  gpmc_driver : ramstix_gpmc_driver generic map(
      DATA_WIDTH           => DATA_WIDTH,
      GPMC_ADDR_WIDTH_HIGH => GPMC_ADDR_WIDTH_HIGH,
      GPMC_ADDR_WIDTH_LOW  => GPMC_ADDR_WIDTH_LOW,
      RAM_SIZE             => RAM_SIZE
    )
    port map (
      clk           => CLOCK_50,
      -- Linux offset: idx 0
      reg0_in       => QD1_out(15 downto 0),       -- LSB
      reg1_in       => QD1_out(31 downto 16),       -- MSB
      
      -- Linux offset: idx 1
      reg2_in       => QD2_out(15 downto 0),  -- LSB
      reg3_in       => QD2_out(31 downto 16),  -- MSB
      
      -- Linux offset: idx 2
      reg4_out      => MC1_in(15 downto 0),      -- LSB
      reg5_out      => MC1_in(31 downto 16),      -- MSB
      
      -- Linux offset: idx 3
      reg6_out      => MC2_in(15 downto 0),  -- LSB
      reg7_out      => MC2_in(31 downto 16),  -- MSB

      GPMC_DATA     => GPMC_DATA,
      GPMC_ADDR     => GPMC_ADDR,
      GPMC_nPWE     => GPMC_nPWE,
      GPMC_nOE      => GPMC_nOE,
      GPMC_FPGA_IRQ => GPMC_FPGA_IRQ,
      GPMC_nCS6     => GPMC_nCS6,
      GPMC_CLK      => GPMC_CLK
    );
    
  MC1 : Motor_Controller 
    generic map(
      32, 2500
    )
    port map ( 
      CLOCK_50,
      reset,
      enable,
      PWM1A,
      PWM1B,
      PWM1C,
      MC1_in
    );
  MC2 : Motor_Controller 
    generic map(
      32, 2500
    )
    port map ( 
      CLOCK_50,
      reset,
      enable,
      PWM2A,
      PWM2B,
      PWM2C,
      MC2_in
    );

  QD1 : QuadratureDecoder 
    generic map(32)
    port map(
      CLOCK_50,
      reset,
      enable,
      QD1_out,
      ENC1A,
      ENC1B
    );
  QD2 : QuadratureDecoder 
    generic map(32)
    port map(
      CLOCK_50,
      reset,       
      enable,
      QD2_out,
      ENC2A,
      ENC2B
    );
end architecture;
