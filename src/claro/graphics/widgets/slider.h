/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */


#ifndef _CLARO_GRAPHICS_WIDGETS_SLIDER_H
#define _CLARO_GRAPHICS_WIDGETS_SLIDER_H
/** \defgroup slider_widget_t Slider Widget
 * \brief The Slider Widget
 * @{
 */

#define assert_only_slider_widget(o,n)	assert_type_check( o, n, "slider_widget" )
#define assert_valid_slider_widget(o,n)	assert_not_null( o, n, "Slider" ); \
						assert_only_slider_widget( o, n )

declare_class( slider_widget );
#define slider_widget_type slider_widget_get_type()

typedef struct slider_widget_ {
    widget_t widget;
} slider_widget_t;


enum cSliderStyle {
	cSliderHorizontal = 0x0,
	cSliderVertical = 0x01,
	cSliderKnob = 0x02,
};

/**
 * \brief Creates a Slider widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new Slider widget object.
 */
CLFEXP object_t *slider_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Sets the range of a slider widget
 * 
 * \param slider A valid slider widget object
 * \param minimum Slider minimum
 * \param maximum Slider maximum
 */
CLFEXP void slider_set_range( object_t *slider, double minimum, double maximum );

/**
 * \brief Sets the value of a slider widget
 * 
 * \param slider A valid slider widget object
 * \param value Slider value
 */
CLFEXP void slider_set_value( object_t *slider, double value );

/**
 * \brief Gets the value of a slider widget
 * 
 * \param slider A valid slider widget object
 * \return Slider value
 */
CLFEXP double slider_get_value( object_t *slider );

/**
 * \brief Sets the orientation of a progress widget
 * 
 * \param slider A valid slider widget object
 * \param style One of the cSliderStyle values
 */
CLFEXP void slider_set_style( object_t *slider, int style );

/*\@}*/
#endif

