/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_song_bg_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:15:13 by achahid-          #+#    #+#             */
/*   Updated: 2024/12/02 22:41:01 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d_bonus.h>

void	play_menu_music(void);
void	stop_menu_music(void);

void	play_menu_music(void)
{
	int		r;

	r = system("cvlc loading/rai_remix.mp3 &");
	(void)r;
}

void	stop_menu_music(void)
{
	int	r;

	r = system("pkill -f loading/rai_remix.mp3");
	(void)r;
}
