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

	#ifdef __linux
	r = system("cvlc loading/rai_remix.mp3 &");
	#endif
	#ifdef __APPLE__
	r = system("afplay loading/rai_remix.mp3 &");
	#endif
	(void)r;
}

void	stop_menu_music(void)
{
	int	r;

	#ifdef __linux
	r = system("pkill -f loading/rai_remix.mp3");
	#endif
	#ifdef __APPLE__
	r = system("pkill -f loading/rai_remix.mp3");
	#endif
	(void)r;
}
