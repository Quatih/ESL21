
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity testbench is

end testbench;


architecture tb of testbench is

component QuadratureEncoder is
 port ( 
	clk   : in std_logic;
	reset  : in std_logic;
	enable : in std_logic;
	GPIO_0		: inout std_logic_vector(wl -1 downto 0);
	GPIO_0_IN	: in    std_logic_vector(1 downto 0);

	);
end component QuadratureEncoder;

signal clk : std_logic := '0';

signal GPIO_0 : std_logic_vector(33 downto 0);
signal GPIO_0_in : std_logic_vector(1 downto 0) := (others => '0');
begin

clk <= not clk after 10 ns;

enc : QuadratureEncoder 
	port map(clk, LED, KEY, SW, GPIO_0, GPIO_0_in, GPIO_1, GPIO_1_in);


process
begin
	
	for count in 0 to 15 loop
		GPIO_0_in(1) <= '1';
		wait for 25 ns;
		GPIO_0_in(0) <= '1';
		wait for 25 ns;
		GPIO_0_in(1) <= '0';
		wait for 25 ns;
		GPIO_0_in(0) <= '0';
		wait for 25 ns;
	end loop;

	for count in 0 to 15 loop
		GPIO_0_in(0) <= '1';
		wait for 25 ns;
		GPIO_0_in(1) <= '1';
		wait for 25 ns;
		GPIO_0_in(0) <= '0';
		wait for 25 ns;
		GPIO_0_in(1) <= '0';
		wait for 25 ns;
	end loop;
end process;

end tb;

