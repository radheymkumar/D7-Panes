<?php
/**
	Tutorials => https://www.fourkitchens.com/blog/article/creating-custom-panels-panes-and-use-substitution-too/
*/

// Show Panel Category Name and Add Pane "My Module Custom Pane"

$plugin = array(
	'single' => TRUE,	// Just do this one, it is needed.
	'title' => t('My Module Custom Pane'), // Title to show up on the pane screen.
	'description' => t('My first module custom pane'), // Description to show up on the pane screen.
	'category' => t('Reab'), //A category to put this under.
	'edit form' => 'mymod_pane_custom_pane_edit_form', // A function that will return the settings form for the pane.
	'render callback' => 'mymod_pane_custom_pane_render', // A function that will return the renderable content.
	'admin info' => 'mymod_pane_custom_pane_admin_info', // A function that will return the information displayed on the admin screen (optional).
	'defaults' => array(	// Array of defaults for the settings form.
    'text' => '',
	),
	'all contexts' => TRUE, // This is NEEDED to be able to use substitution strings in your pane.
);

/**
 * An edit form for the pane's settings.
 * create the edit form, render the pane, and create the admin info display.
 */
function mymod_pane_custom_pane_edit_form($form, &$form_state) {
	$conf = $form_state['conf'];
	$form['text'] = array(
		'#type' => 'textfield',
		'#title' => t('Panel Text'),
		'#description' => t('Text to display, it may use substitution strings'),
		'#default_value' => $conf['text'],
	);
	return $form;
}

/**
 * Submit function, note anything in the formstate[conf] automatically gets saved
 * Notice, the magic that automatically does that for you.
 */
function mymod_pane_custom_pane_edit_form_submit(&$form, &$form_state) {
	foreach (array_keys($form_state['plugin']['defaults']) as $key) {
		if (isset($form_state['values'][$key])) {
			$form_state['conf'][$key] = $form_state['values'][$key];
		}
	}
	// use sort
	/*$form_state['conf']['term'] = $form_state['values']['term'];
  $form_state['conf']['view_mode'] = $form_state['values']['view_mode'];*/
}

/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 *  $subtype - file name "mymod_custom_pane" 
 *  $conf - form value "[text]=>radhey"
 *  $arg - empty
 *  $contexts - panelizer info
 */
function mymod_pane_custom_pane_render($subtype, $conf, $arg, $contexts) {
	
	$node_data = $contexts['panelizer']->data;
	$node_title = $node_data->title;

	$content_text = $conf['text'];
	$content_image = image_style_url('large', $node_data->field_hero_image['und'][0]['uri']);
	
	$breadcrumb=drupal_get_breadcrumb();
	foreach($breadcrumb as $crumb){
	  $breadcrumb_output .= $crumb.' / ';
  }
  $breadcrumb_output.= $node_data->type;

  switch($node_data->type){
  	case 'discipline':
  		$showbreadcrumb = true;
  			break;
  }

  	$content = "<hr>";
		$content .= '<div class="first-my-pane">';
		if($showbreadcrumb){
			$content .='<div class="breadcrumb">'.$breadcrumb_output.'</div>';
		}	
		$content .= '<h3>Page Title -' . $node_title . '</h3>';
		$content .= '<img src="'.$content_image.'" alt="'. $node_title .'"/>';
		$content .= '<h4>'. $content_text .'</h4>';
		$content .= '</div>';
		if(isset($node_data->field_youtube['und'][0]['url'])) {
			$content .= '<a href="'.$node_data->field_youtube['und'][0]['url'].'"><i class="fa fa-youtube" aria-hidden="true" style="color:red;font-size:48px;" ></i></a>';
		}

		$content .= render(field_view_field('node',$node_data,'field_youtube', array('label'=>'hidden')));

		$content .= '<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3559.0055995757257!2d75.81546145023236!3d26.87156318306263!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x396db67cda4edb8f%3A0x44a26758d0377e21!2sDotsquares+Jhalana+Office!5e0!3m2!1sen!2sin!4v1534576676148" width="100%" height="250" frameborder="0" style="border:0" allowfullscreen></iframe>';

		$content .= '<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">';

		$content .= '<div class="heal-code-footer"><div class="container"><a href="#"><i class="fa fa-car"></i> GO to HOME PAGE</a></div></div>';
		$content .= views_embed_view('latest_post','panel_pane_1');

		$content .= "<hr>";
	$block = new stdClass();
	// initial content is blank
	//$block->title = t('This is my title');
	$block->content = $content;

	return $block;

}

/**
 * 'admin info' callback for panel pane.
 *  $subtype - file name "mymod_custom_pane" 
 *  $conf - form value "[text]=>radhey"
 *  $contexts - panelizer info
 */
function mymod_pane_custom_pane_admin_info($subtype, $conf, $contexts) {
	if (!empty($conf)) {
		$block = new stdClass();
		$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
		$block->content = $conf['text'];
		return $block;
	}
}