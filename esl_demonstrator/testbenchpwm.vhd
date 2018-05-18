library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity testbench_pwm is
generic (wl : natural := 32);
end testbench_pwm;


architecture tb of testbench_pwm is

component PWM is
generic (wl : natural := 32);
 port ( 
	clk   : in std_logic;
	reset  : in std_logic;
	enable : in std_logic;
	
	INA		: out std_logic;	--INA input of the H-Bridge.
	INB		: out std_logic;	--INB input of the H-bridge.
	C		: Out std_logic;	--PWM input of the H-Bridge.
	
	GPIO_0		: in std_logic_vector(wl -1 downto 0);
	GPIO_0_IN	: in    std_logic_vector(1 downto 0)

	);
end component PWM;

signal clk : std_logic := '0';
signal reset : std_logic := '0';
signal enable : std_logic := '1';

signal GPIO_0 : std_logic_vector(wl-1 downto 0) := (others => '0');
signal GPIO_0_in : std_logic_vector(1 downto 0) := (others => '0');
signal GPIO : std_logic_vector(wl-1 downto 0) := (others => '0');

begin

clk <= not clk after 10 ns;
GPIO_0 <= GPIO;


process 
begin
  GPIO(wl-1 downto 10) <= (others => '0');
	--Start with clockwise
	GPIO(8) <= '1';
	GPIO(9) <= '0';
	
	for count in 0 to 25 loop
		GPIO(7 downto 0) <= std_logic_vector(to_unsigned(count*10, 8));
		wait for 50000 ns; -- 2 pwm periods
	end loop;
	--Continue with counterclockwise
	--GPIO_0(8) <= '0';
	--GPIO_0(9) <= '1';
	
	--for count in 0 to 25 loop
	--	GPIO_0(7 downto 0) <= std_logic_vector(to_unsigned(count*10, 8));
	--	wait for 5000 ns;
	--end loop;
	
end process;

PWMmodule : PWM 
		port map (
			clk => clk,
			reset => reset,
			enable => enable,
			GPIO_0 => GPIO_0,
			GPIO_0_IN => GPIO_0_in
			-- Map your encoder here to the I/O
		);
end tb;
