#include "ft_printf.h"

int	ft_puthex(unsigned long n, char format)
{
	int	count;

	count = 0;
	if (n >= 16)
		count += ft_puthex(n / 16, format);
	if (n % 16 < 10)
		count += ft_putchar ((n % 16) + '0');
	else
	{
		if (format == 'x')
			count += ft_putchar((n % 16) - 10 + 'a');
		else if (format == 'X')
			count += ft_putchar((n % 16) - 10 + 'A');
	}
	return (count);
}
