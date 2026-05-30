/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:54:42 by rgomes-g          #+#    #+#             */
/*   Updated: 2025/07/31 18:47:17 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int num, int fd)
{
	long int	nwnum;

	nwnum = num;
	if (nwnum < 0)
	{
		nwnum *= -1;
		ft_putchar_fd('-', fd);
	}
	if (nwnum > 9)
		ft_putnbr_fd(nwnum / 10, fd);
	ft_putchar_fd(nwnum % 10 + '0', fd);
}
