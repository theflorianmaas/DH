<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');


//date_default_timezone_set('Europe/Rome');
 
class Test extends CI_Controller {
 
    function __construct()
    {
        parent::__construct();
 
        /* Standard Libraries of codeigniter are required */
        $this->load->database();
		$this->db->reconnect();
        $this->load->helper('url');
        /* ------------------ */ 
 
        $this->load->library('grocery_CRUD');
 
    }
    
       	function v_output($output = null)
	{
		$this->load->view('pages/main', $output);	
	}
	
	function v_view($output = null, $page = null)
	{
		$this->load->view('pages/'. $page, $output);	
	}
    
	
	function index()
	{
		$this->v_output((object)array('output' => '' , 'js_files' => array() , 'css_files' => array()));
	}
}
