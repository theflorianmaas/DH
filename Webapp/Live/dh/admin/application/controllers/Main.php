<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');


//date_default_timezone_set('Europe/Rome');
 
class Main extends CI_Controller {
 
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


/*---------------------------------------------------------------------------*/	
/*---------------------------------------------------------------------------*/		
	/*
	* CUSTOM DEPENDENT DROPDOWN
	*/
//Ok
function tbsensor()
	{
	$title = 'Sensor';
    $crud = new grocery_CRUD();
    $crud->set_table('tbsensor')  
         ->set_subject($title)
         ->columns('id',
				'descr',
			'tbnode_id',
		  'tbstatus_id',
	  'tbsensortype_id',
		   'pin_number',
		 'currentvalue',
		'addtohomepage',
	       'tbchart_id',
           'lastupdate',
        'currentstatus')
		 ->display_as('tbnode_id','Nodo')
		 ->display_as('descr','Nome sensore')
		 ->display_as('tbsensortype_id','Tipo')		
		 ->display_as('tbstatus_id','Stato')
		 ->display_as('pin_number','Numero pin sensore');
		
		$crud->set_primary_key('id','_tbstatus');
		$crud->set_primary_key('id','_tbsensortype');
		$crud->set_primary_key('id','_tbchart');
		$crud->set_relation('tbnode_id','tbnode','descr');
		$crud->set_relation('tbstatus_id','_tbstatus','wording');
		$crud->set_relation('tbsensortype_id','_tbsensortype','wording');
		$crud->set_relation('tbchart_id','_tbchart','wording');
		$crud->set_relation('currentstatus','_tbstatus','wording');

		 
     $crud->fields('descr','tbnode_id','tbstatus_id','tbsensortype_id','tbchart_id','pin_number','addtohomepage');
     $crud->required_fields('id','descr','tbnode_id','tbstatus_id','tbsensortype_id','pin_number','addtohomepage');
 
    $output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
    
	}	
	
function tbtrigger()
	{	
	$title = 'Trigger';
	$crud = new grocery_CRUD();
    $crud->set_table('tbtrigger')  
         ->set_subject($title)
         ->columns('id',
					'tbtriggertype_id',
					'tbevent_id',
					'tbactiongroup_id',
					'tbstatus_id',
					'tbtimer_id',
					'sendalert',
					'descr')
		 ->display_as('tbtriggertype_id','Trigger Type')
		 ->display_as('tbevent_id','Event')
		 ->display_as('tbactiongroup_id','Action group')
		 ->display_as('tbstatus_id','Status')
		 ->display_as('tbtimer_id','Timer')
		 ->display_as('sendalert','Send alert');

	$crud->set_primary_key('id','_tbstatus');
	$crud->set_primary_key('id','_tbtriggertype');
		
	$crud->set_relation('tbstatus_id','_tbstatus','wording');
	$crud->set_relation('tbtriggertype_id','_tbtriggertype','wording');
	$crud->set_relation('currentstatus','_tbstatus','wording');
	$crud->set_relation('tbevent_id','tbevent','descr');
	$crud->set_relation('tbtimer_id','tbtimer','descr');
	$crud->set_relation('tbactiongroup_id','tbactiongroup','descr');
		
	$crud->fields('id','tbtriggertype_id','tbstatus_id','tbevent_id','tbtimer_id','tbactiongroup_id','sendalert','descr');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
        
    }	

//Ok	
function tbactiongroup()
	{
 $title = 'Action Group';
    $crud = new grocery_CRUD();
    $crud->set_table('tbactiongroup')  
         ->set_subject($title)
         ->columns('id','descr')
         ->display_as('descr','Description');
		
	$crud->fields('id','descr');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

//Ok	
function tbactionitem()
	{
 $title = 'Action item';
    $crud = new grocery_CRUD();
    $crud->set_table('tbactionitem')  
         ->set_subject($title)
         ->columns('id','tbactiongroup_id','tbactuators_id','actiontrue','actionfalse','sequence')
         ->display_as('tbactiongroup_id','Action group')
         ->display_as('tbactuators_id','Actuator')
         ->display_as('actiontrue','Action in range')
         ->display_as('actionfalse','Action out of range')
         ->display_as('sequence','Sequence');
		 
	$crud->set_relation('tbactiongroup_id','tbactiongroup','descr');	
	$crud->set_relation('tbactuators_id','tbactuator','descr');		
	$crud->set_relation('actiontrue','tbaction','descr');	
	$crud->set_relation('actionfalse','tbaction','descr');	
		
	$crud->fields('id','tbactiongroup_id','tbactuators_id','actiontrue','actionfalse','sequence');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}		
	
//Ok	
function tbevent()
	{
$title = 'Event'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbevent')  
         ->set_subject($title)
         ->columns('id','descr','currentstatus')
         ->display_as('descr','Description')
         ->display_as('currentstatus','Current status');
	$crud->set_primary_key('id','_tbstatus');
		
	$crud->set_relation('currentstatus','_tbstatus','wording');
	$crud->fields('id','descr');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	
//Ok		
function tbeventlogic()
	{
	$title = 'Event logic';
    $crud = new grocery_CRUD();
    $crud->set_table('tbeventlogic')  
         ->set_subject($title)
         ->columns('id','tbevent_id','tbsensor_id','valmin','valmax','sequence','logic','bracket')
         ->display_as('tbevent_id','Event')
         ->display_as('tbsensor_id','Sensor')
         ->display_as('valmin','Min value')
         ->display_as('valmax','Max value')
         ->display_as('sequence','Sequence')
         ->display_as('logic','Logic')
         ->display_as('bracket','Bracket');
	
	$crud->field_type('logic','dropdown',array('0' => 'AND', '1' => 'OR'));	
	$crud->field_type('bracket','dropdown',array('(' => '(', ')' => ')'));			
	
	$crud->set_relation('tbevent_id','tbevent','descr');
	$crud->set_relation('tbsensor_id','tbsensor','descr');
	
	$crud->order_by('tbevent_id','sequence');
	
	$crud->fields('id','tbevent_id','tbsensor_id','valmin','valmax','sequence','logic','bracket');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}

//Ok
function tbtimer()
	{
	$title = 'Timer';
	$crud = new grocery_CRUD();
    $crud->set_table('tbtimer')  
         ->set_subject($title)
         ->columns('id','descr','tbtimerdaily_id','tbtimerweekly_id','tbtimermonthly_id')
         ->display_as('descr','Description')
         ->display_as('tbtimerdaily_id','Daily')
         ->display_as('tbtimermonthly_id','Montly')
         ->display_as('tbtimerweekly_id','Weekly');
    
    $crud->set_relation('tbtimerdaily_id','tbtimerdaily','descr');  
    $crud->set_relation('tbtimerweekly_id','tbtimerweekly','descr');  
    $crud->set_relation('tbtimermonthly_id','tbtimermonthly','descr');     
         
    $crud->fields('id','descr','tbtimerdaily_id','tbtimerweekly_id','tbtimermonthly_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);

	}	

//Ok	
function tbtimerdaily()
	{
	$title = 'Daily timer';
	$crud = new grocery_CRUD();
    $crud->set_table('tbtimerdaily')  
         ->set_subject($title)
         ->columns('id','descr','timestart','timeend')
         ->display_as('descr','Description')
         ->display_as('timestart','Start time')
         ->display_as('timeend','End time');
    
         
    $crud->fields('id','descr','timestart','timeend');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);

	}

//Ok
function tbtimerweekly()
	{
	$title = 'Weekly timer';
	$crud = new grocery_CRUD();
    $crud->set_table('tbtimerweekly')  
         ->set_subject($title)
         ->columns('id','descr','wd0','wd1','wd2','wd3','wd4','wd5','wd6')
         ->display_as('descr','Description')
         ->display_as('wd0','Monday')
         ->display_as('wd1','Tuesday')
         ->display_as('wd2','Wednesday')
         ->display_as('wd3','Thursday')
         ->display_as('wd4','Friday')
         ->display_as('wd5','Sadurday')
         ->display_as('wd6','Sunday');
              
    $crud->field_type('wd0','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('wd1','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('wd2','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('wd3','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('wd4','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('wd5','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('wd6','dropdown',array('0' => 'Off', '1' => 'On'));	
             
    $crud->fields('id','descr','wd0','wd1','wd2','wd3','wd4','wd5','wd6');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);

	}								

//Ok  	
function tbtimermonthly()
	{
	$title = 'Monthly timer';
	$crud = new grocery_CRUD();
    $crud->set_table('tbtimermonthly')  
         ->set_subject($title)
         ->columns('id','descr','m1','m2','m3','m4','m5','m6','m7','m8','m9','m10','m11','m12')
         ->display_as('descr','Description')
         ->display_as('m1','January')
         ->display_as('m2','February')
         ->display_as('m3','March')
         ->display_as('m4','April')
         ->display_as('m5','May')
         ->display_as('m6','June')
         ->display_as('m7','July') 
         ->display_as('m8','August')
         ->display_as('m9','September')
         ->display_as('m10','October')        
         ->display_as('m11','November')   
         ->display_as('m12','December') ;
              
    
    $crud->field_type('m1','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('m2','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('m3','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('m4','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('m5','dropdown',array('0' => 'Off', '1' => 'On'));	
    $crud->field_type('m6','dropdown',array('0' => 'Off', '1' => 'On'));	
	$crud->field_type('m7','dropdown',array('0' => 'Off', '1' => 'On'));	
	$crud->field_type('m8','dropdown',array('0' => 'Off', '1' => 'On'));	
	$crud->field_type('m9','dropdown',array('0' => 'Off', '1' => 'On'));	
	$crud->field_type('m10','dropdown',array('0' => 'Off', '1' => 'On'));	
	$crud->field_type('m11','dropdown',array('0' => 'Off', '1' => 'On'));	
	$crud->field_type('m12','dropdown',array('0' => 'Off', '1' => 'On'));	    
             
    $crud->fields('id','descr','m1','m2','m3','m4','m5','m6','m7','m8','m9','m10','m11','m12');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);

	}				  	
 
//ok 
function tbactuator()
	{
	$title = 'Actuator';
	$crud = new grocery_CRUD();
    $crud->set_table('tbactuator')  
         ->set_subject($title)
         ->columns('id','tbnode_id','tbactuatortype_id','descr','pinnumber','fadingtime','temptime')
         ->display_as('descr','Description')
         ->display_as('tbactuatortype_id','Actuator type')
         ->display_as('tbnode_id','Node')
         ->display_as('pinnumber','Pin number')
         ->display_as('fadingtime','Fading time (sec.)') 
         ->display_as('temptime','Timer time (sec.)');
    
   	$crud->set_primary_key('id','_tbstatus');
	$crud->set_primary_key('id','_tbactuatortype');
	$crud->set_relation('tbnode_id','tbnode','descr');
	$crud->set_relation('currentstatus','_tbstatus','wording');
	$crud->set_relation('tbactuatortype_id','_tbactuatortype','wording');
		         
    $crud->fields('id','tbnode_id','tbactuatortype_id','descr','pinnumber','fadingtime','temptime');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);

	}
	
//Ok	
function tblocation()
	{
 $title = 'Location';
    $crud = new grocery_CRUD();
    $crud->set_table('tblocation')  
         ->set_subject($title)
         ->columns('id','descr','users_user_id')
         ->display_as('descr','Description')
         ->display_as('users_user_id','User');
	
	$crud->set_relation('users_user_id','users','user_name');
	$crud->fields('id','descr','users_user_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}		

//Ok	
function tbnode()
	{
 $title = 'Node';
    $crud = new grocery_CRUD();
    $crud->set_table('tbnode')  
         ->set_subject($title)
         ->columns('id','tbroom_id','descr','tbnodetype_id','tbstatus_id','xbee_low_address','xbee_high_address')
		 ->fields('id','tbroom_id','descr','tbnodetype_id','tbstatus_id','xbee_low_address','xbee_high_address')
		  ->display_as('tbroom_id','Room')
		  ->display_as('descr','Node name')
		  ->display_as('tbnodetype_id','Node type')
		  ->display_as('tbstatus_id','Status')
		  ->display_as('xbee_low_address','Zigbee Low address')
		  ->display_as('xbee_high_address','Zigbee High address'); 
	
	$crud->set_primary_key('id','_tbnodetype');
	$crud->set_primary_key('id','_tbstatus');
	
	$crud->set_relation('tbroom_id','tbroom','descr');
	$crud->set_relation('tbnodetype_id','_tbnodetype','wording');
	$crud->set_relation('tbstatus_id','_tbstatus','wording');
	
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

//Ok	
function tbroom()
	{
 $title = 'Room';
    $crud = new grocery_CRUD();
    $crud->set_table('tbroom')  
         ->set_subject($title)
         ->columns('id','tblocation_id','descr','tbicon_id')
         ->display_as('tblocation_id','Location')
         ->display_as('descr','Room name')
		 ->display_as('tbicon_id','Icon');
	
	$crud->set_relation('tblocation_id','tblocation','descr');
	$crud->set_relation('tbicon_id','tbicon','descr');
	$crud->fields('id','tblocation_id','descr','tbicon_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}		
	
	
	
/*---------------------------------------------------------------------------*/	
/* Master tables                                                             */
/*---------------------------------------------------------------------------*/	

//Ok
function tbnodetype()
	{
 $title = 'Node Type';
    $crud = new grocery_CRUD();
    $crud->set_table('tbnodetype')  
         ->set_subject($title)
         ->columns('id','tbtranslation_id','tbicon_id','tbcolor_id','scalemin','scalemax','note')
		 ->fields('id','tbtranslation_id','tbicon_id','tbcolor_id','scalemin','scalemax','note')
		 ->display_as('tbtranslation_id','Name')
		 ->display_as('tbicon_id','Icon')
		 ->display_as('tbcolor_id','Color')
		 ->display_as('scalemin','Scale Min value')
		 ->display_as('scalemax','Scale Max value')
		 ->display_as('note','Notes') ;
	
	$crud->set_primary_key('id','vwtranslation');
	
	$crud->set_relation('tbtranslation_id','vwtranslation','wording');
	$crud->set_relation('tbicon_id','tbicon','icon');
	$crud->set_relation('tbcolor_id','tbcolor','hexcode');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	
	
//Ok
function tbsensortype()
	{
$title = 'Sensor Type'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbsensortype')  
         ->set_subject($title)
         ->columns('id','tbtranslation_id','tbconvformula_id','tbicon_id','tbcolor_id','scalemin','scalemax','timer','tbum_id','notes','smooth')
		 ->fields('id','tbtranslation_id','tbconvformula_id','tbicon_id','tbcolor_id','scalemin','scalemax','timer','tbum_id','notes','smooth')
		 ->display_as('tbtranslation_id','Name')
		 ->display_as('tbconvformula_id','Conversion Formula')
		 ->display_as('tbicon_id','Icon')
		 ->display_as('tbcolor_id','Color')
		 ->display_as('scalemin','Scale Min value')
		 ->display_as('scalemax','Scale Max value')
		 ->display_as('timer','Store into history')
		 ->display_as('tbum_id','Unit measure')
		 ->display_as('note','Notes') 
		 ->display_as('smooth','Chart line smoothing') ;
		 
	$crud->set_primary_key('id','vwtranslation');
	
	$crud->set_relation('tbtranslation_id','vwtranslation','wording');
	$crud->set_relation('tbconvformula_id','tbconvformula','formula');
	$crud->set_relation('tbicon_id','tbicon','descr');
	$crud->set_relation('tbcolor_id','tbcolor','hexcode');
	$crud->set_relation('timer','tbparam','descr',"ptype='T'");
	$crud->set_relation('tbum_id','tbum','descr');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}		


//Ok
function tbactuatortype()
	{
	$title = 'Actuator type';
	$crud = new grocery_CRUD();
    $crud->set_table('tbactuatortype')  
         ->set_subject($title)
         ->columns('id','tbtranslation_id','tbicon_id','tbcolor_id','tboutput_id','valueon','valueoff','websize')
         ->display_as('tbtranslation_id','Description')
         ->display_as('tbicon_id','Icon')
         ->display_as('tbcolor_id','Color')
         ->display_as('tboutput_id','Output type')
         ->display_as('valueon','Value On') 
         ->display_as('valueoff','Value Off')		 
		 ->display_as('websize','Field height');

         
   $crud->set_primary_key('id','vwtranslation');     
   $crud->set_relation('tbtranslation_id','vwtranslation','wording');   
   $crud->set_relation('tbicon_id','tbicon','descr');   
   $crud->set_relation('tbcolor_id','tbcolor','hexcode'); 
   $crud->set_relation('tboutput_id','tboutput','descr');     
         
   $crud->fields('id','tbtranslation_id','tbicon_id','tbcolor_id','tboutput_id','valueon','valueoff','websize');  
  
   $output = $crud->render();
	$output->title = $title;
    $this->v_output($output);	
   }

//Ok   
function tbaction()
	{
$title = 'Action'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbaction')  
         ->set_subject($title)
         ->columns('id','descr','value');
		 
	$crud->fields('id','descr','value');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	
	
//Ok
function tbmenu()
	{
	$title = 'Initial Menu'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbmenu')
         ->set_subject($title)
       ->columns('page','tbicon_id','tbtranslation_id');
		 
	$crud->set_relation('tbicon_id','tbicon','descr');	
	$crud->set_relation('tbtranslation_id','vwtranslation','wording');	
	$crud->fields('page','tbicon_id','tbtranslation_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	
	
//Ok   
function tbfunctions()
	{
$title = 'Function'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbfunction')  
         ->set_subject($title)
         ->columns('id','descr','tbicon_id')
	->display_as('tbicon_id','Icon');
		 
	$crud->set_relation('tbicon_id','tbicon','descr');	 
	$crud->fields('id','descr','tbicon_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

//Ok - da verificare
function tbfunctiontriggers()
	{
$title = 'Function Trigger'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbfunctiontrigger')  
         ->set_subject($title)
         ->columns('tbfunction_id','tbtrigger_id')
	->display_as('tbfunction_id','Function')
	->display_as('tbtrigger_id','Trigger');
		 
	$crud->set_relation('tbfunction_id','tbfunction','descr');	 
	$crud->set_relation('tbtrigger_id','tbtrigger','descr');	 
	$crud->fields('tbfunction_id','tbtrigger_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}
	
	
//Ok - da verificare
function tbchart()
	{
 $title = 'Chart type';
    $crud = new grocery_CRUD();
    $crud->set_table('tbchart')  
         ->set_subject($title)
         ->columns('id','tbtranslation_id','tbicon_id','smooth')
		 ->display_as('tbicon_id','Icon','Chart line smoothing');
	
	$crud->set_primary_key('translation_id','_tbchart');
	$crud->set_relation('tbtranslation_id','_tbchart','wording');
	$crud->set_relation('tbicon_id','tbicon','descr');
	$crud->fields('id','tbtranslation_id','tbicon_id','smooth');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

//Ok	
function tbcolor()
	{
$title = 'Colors'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbcolor')  
         ->set_subject($title)
         ->columns('id','hexcode');
	
	$crud->fields('id','hexcode');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

//Ok
function tbconvFormula()
	{
$title = 'Conversion Formula'; 
    $crud = new grocery_CRUD();
	$crud->set_table('tbconvFormula')  
         ->set_subject($title)
         ->columns('id','formula');
	
	$crud->fields('id','formula');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}

//Ok
function tbicon()
	{
$title = 'Icon'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbicon')  
         ->set_subject($title)
         ->columns('id','icon','descr','code');
	
	$crud->fields('id','icon','descr','code');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

//Ok
function tblanguage()
	{
$title = 'Language'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tblanguage')  
         ->set_subject($title)
         ->columns('id','descr');
	
	$crud->fields('id','descr');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

//Ok	
function tboutput()
	{
$title = 'Output type'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tboutput')  
         ->set_subject($title)
         ->columns('id','descr');
	
	$crud->fields('id','descr');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}		

function tbproctl()
	{
	$title = 'Proctl'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbproctl')  
         ->set_subject($title)
         ->columns('id','sts','proc');
	
	$crud->fields('id','sts','proc');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}
	
function tbparam()
	{
$title = 'Parameters'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbparam')  
         ->set_subject($title)
         ->columns('id','pvalue','descr','ptype','pindex');
	
	$crud->fields('id','pvalue','descr','ptype','pindex');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	
	
//Ok	
function tbstatus()
	{
$title = 'Status'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbstatus')  
         ->set_subject($title)
         ->columns('id','tbtranslation_id','tbicon_id');
	
	$crud->set_primary_key('id','vwtranslation');
	$crud->set_relation('tbtranslation_id','vwtranslation','wording');
	$crud->set_relation('tbicon_id','tbicon','icon');
	$crud->fields('id','tbtranslation_id','tbicon_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

//Ok	
function tbtranslation()
	{
 $title = 'Translation';
    $crud = new grocery_CRUD();
    $crud->set_table('tbtranslation')  
         ->set_subject($title)
         ->columns('id');
	
	$crud->fields('id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}

//Ok	
function tbtranslationitem()
	{
$title = 'Translation item'; 
    $crud = new grocery_CRUD();
    $crud->set_table('tbtranslationitem')  
         ->set_subject($title)
         ->columns('tblanguage_id','tbtranslation_id','wording');
	
	$crud->set_relation('tblanguage_id','tblanguage','descr');

	$crud->fields('tblanguage_id','tbtranslation_id','wording');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}		
	
//Ok	
function tbtriggertype()
	{
 $title = 'Trigger Type';
    $crud = new grocery_CRUD();
    $crud->set_table('tbtriggertype')  
         ->set_subject($title)
         ->columns('id','tbtranslation_id','tbicon_id');
	
	$crud->set_primary_key('id','vwtranslation');
	$crud->set_relation('tbtranslation_id','vwtranslation','wording');
	$crud->set_relation('tbicon_id','tbicon','descr');
	$crud->fields('id','tbtranslation_id','tbicon_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

//Ok	
function users()
	{
    $title = "Users";
    $crud = new grocery_CRUD();
    $crud->set_table('users')  
         ->set_subject($title)
         ->columns('user_id','user_name','user_email','user_active','user_registration_ip','user_language');
	
	$crud->set_relation('user_language','tblanguage','descr');
	$crud->fields('user_id','user_name','user_email','user_active','user_registration_ip','user_language');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}				

//Ok	
function tbcustomparams()
	{
    $title = "Custom Parameters";
    $crud = new grocery_CRUD();
    $crud->set_table('tbcustomparams')  
         ->set_subject($title)
         ->columns('id','idx','tbtranslation_id');
	
	$crud->set_relation('tbtranslation_id','vwtranslation','wording');
	
	$crud->fields('id','idx','tbtranslation_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}
	
	
function tbsubsystems()
	{
    $title = "Subsystems";
    $crud = new grocery_CRUD();
    $crud->set_table('tbsubsystem')  
         ->set_subject($title)
         ->columns('id','descr','tbsystem_id','tbicon_id');
	
	$crud->set_relation('tbsystem_id','tbsystem','descr');
	$crud->set_relation('tbicon_id','tbicon','descr');
		
	$crud->fields('id','descr','tbsystem_id','tbicon_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	
	
function tbsubsystemactuators()
	{
    $title = "Subsystemactuators";
    $crud = new grocery_CRUD();
    $crud->set_table('tbsubsystemactuator')  
         ->set_subject($title)
         ->columns('tbsubsystem_id','tbactuators_id');
	
	$crud->set_relation('tbsubsystem_id','tbsubsystem','descr');
	$crud->set_relation('tbactuators_id','tbactuator','descr');
		
	$crud->fields('tbsubsystem_id','tbactuators_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}		
	
function tbsubsystemsensors()
	{
    $title = "Subsystemsensors";
    $crud = new grocery_CRUD();
    $crud->set_table('tbsubsystemsensor')  
         ->set_subject($title)
         ->columns('tbsubsystem_id','tbsensor_id');
	
	$crud->set_relation('tbsubsystem_id','tbsubsystem_id','descr');
	$crud->set_relation('tbsensor_id','tbsensor','descr');
		
	$crud->fields('tbsubsystem_id','tbsensor_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	

function tbsystems()
	{
    $title = "Systems";
    $crud = new grocery_CRUD();
    $crud->set_table('tbsystem')  
         ->set_subject($title)
         ->columns('id','descr','users_user_id','tbicon_id');
	
	$crud->set_relation('users_user_id','users','user_name');
	$crud->set_relation('tbicon_id','tbicon','descr');
		
	$crud->fields('id','descr','users_user_id','tbicon_id');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}	
	
function tbums()
	{
    $title = "Unit measures";
    $crud = new grocery_CRUD();
    $crud->set_table('tbum')  
         ->set_subject($title)
         ->columns('id','descr');
		
	$crud->fields('id','descr');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}		
	
function tbcoordinates()
	{
    $title = "Positions";
    $crud = new grocery_CRUD();
    $crud->set_table('tbcoordinates')  
         ->set_subject($title)
         ->columns('id','x','y','w','h','type','status');
		
	$crud->fields('id','x','y','w','h','type','status');
	
	$output = $crud->render();
	$output->title = $title;
    $this->v_output($output);
 
	}		
		

/*---------------------------------------------------------------------------*/	
/*---------------------------------------------------------------------------*/			
	
}
?>
