library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity Motor_Controller is
 generic (wl : natural := 32;
	  period : natural := 2500);
 port ( 
	clk   : in std_logic;		--System clock, 50 MHz
	reset  : in std_logic;		
	enable : in std_logic;
	
	--Outputs
	INA		: out std_logic;	--INA input of the H-Bridge.
	INB		: out std_logic;	--INB input of the H-bridge.
	C		: Out std_logic;	--PWM input of the H-Bridge.
	GPIO_0		: in std_logic_vector(wl -1 downto 0)

	);
end Motor_Controller;

--The system clock has a period of 1/50 Mhz = 20 ns.
--To achieve a PWM output frequency of 20 kHz (T = 50us), one period should last 50us/20ns = 2500 cycles. 
--With an 8-bit input signal, the output high-time can be varied by approximately 10 cycles.
--The input of the PWM module is a duty-cycle expressed in an 8-bit value -> 0 - 100% = 0 - 255.
--The INA and INB inputs are directly routed from the 2 bit input.

architecture behavior of Motor_Controller is
	--signal dc: std_logic_vector(7 downto 0) := (others => '0');
	signal dc: integer range 0 to 255 := 0;
	signal dc_old: integer range 0 to 255 := 0;
	
	--signal cyclecount: std_logic_vector(11 downto 0) := (others => '0');	-- 12-bit signal to hold the elapsed pulses.
	signal cyclecount: integer range 0 to period := 0;							-- 12-bit signal to hold the elapsed pulses.
	
	signal pulsehigh: integer range 0 to period := 0;
begin

	process(clk, reset)
		variable mode : std_logic_vector(1 downto 0) := (others => '0');
	begin 
		if reset = '1' then 
			--Brake to ground and set the PWM output to 0.
			INA <= '0';
			INB <= '0';
			C <= '0';
			dc <= 0;
			dc_old <= 0;
			cyclecount <= 0;
			pulsehigh <= 0;
		elsif rising_edge(clk) then
			if enable  = '1' then
				dc <= to_integer(unsigned(GPIO_0(7 downto 0)));
				if not(dc = dc_old) then
					pulsehigh <= integer(dc * 10);		--Determine the amount of pulses the signal needs to be high.
				end if;
				mode := GPIO_0(9 downto 8);
				--Set INA and INB
				INA <= mode(0);
				INB <= mode(1);
		
			
				if ((cyclecount < pulsehigh) and not(mode(0) = mode(1))) then	
					C <= '1';
				else
					C <= '0';
				end if;
			
				--Reset the counted cycles.
				if cyclecount = period then
					cyclecount <= 0;
				else
					cyclecount <= cyclecount + 1;
				end if;
			
				dc_old <= dc;
			end if;
		end if;
	
	end process;
	
end behavior;
