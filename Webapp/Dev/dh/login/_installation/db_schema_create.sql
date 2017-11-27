-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema dh
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `dh` ;

-- -----------------------------------------------------
-- Schema dh
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `dh` DEFAULT CHARACTER SET utf8 COLLATE utf8_bin ;
USE `dh` ;

-- -----------------------------------------------------
-- Table `dh`.`_tbremotetrigger`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbremotetrigger` (
  `trigger_id` INT(11) NOT NULL COMMENT '',
  `triggergroup_id` INT(11) NOT NULL COMMENT '',
  `triggeritem_id` INT(11) NOT NULL COMMENT '',
  `trigger_type` INT(11) NOT NULL COMMENT '',
  `node_id` INT(11) NULL DEFAULT NULL COMMENT '',
  `sensor_id` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `sensor_pin` INT(11) NOT NULL COMMENT '',
  `actuator_pin` INT(11) NOT NULL COMMENT '',
  `valuemin` INT(11) NOT NULL COMMENT '',
  `valuemax` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `status` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `output` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `actiontrue` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `actionfalse` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `temptime` INT(11) NOT NULL COMMENT '',
  `fadingtime` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `currentstatus` INT(11) NOT NULL DEFAULT 0 COMMENT '')
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `dh`.`_tbtemptrigger`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbtemptrigger` (
  `trigger_id` INT(11) NOT NULL COMMENT '',
  `triggergroup_id` INT(11) NOT NULL COMMENT '',
  `triggeritem_id` INT(11) NOT NULL COMMENT '',
  `node_id` INT(11) NULL DEFAULT NULL COMMENT '',
  `actuator_pin` INT(11) NULL DEFAULT NULL COMMENT '',
  `action` INT(11) NULL DEFAULT NULL COMMENT '',
  `output` INT(11) NULL DEFAULT NULL COMMENT '',
  `fadingtime` INT(11) NULL DEFAULT NULL COMMENT '',
  `temptime` INT(11) NULL DEFAULT NULL COMMENT '',
  `status` INT(11) NULL DEFAULT '0' COMMENT '',
  PRIMARY KEY (`trigger_id`, `triggergroup_id`, `triggeritem_id`)  COMMENT '')
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`debug`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`debug` (
  `debug` VARCHAR(500) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NULL DEFAULT NULL COMMENT '')
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbaction`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbaction` (
  `id` INT(11) NOT NULL COMMENT '',
  `descr` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `value` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbactiongroup`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbactiongroup` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 4
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbcolor`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbcolor` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `hexCode` VARCHAR(10) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 14
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbicon`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbicon` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `icon` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `descr` VARCHAR(45) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 27
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tboutput`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tboutput` (
  `id` INT(11) NOT NULL COMMENT '',
  `descr` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbtranslation`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbtranslation` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 44
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbactuatortype`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbactuatortype` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `tbTranslation_id` INT(11) NOT NULL COMMENT '',
  `tbIcon_id` INT(11) NOT NULL COMMENT '',
  `tbColor_id` INT(11) NOT NULL COMMENT '',
  `tbOutput_id` INT(11) NOT NULL COMMENT '',
  `valueon` INT NOT NULL DEFAULT 1 COMMENT '',
  `valueoff` INT NOT NULL DEFAULT 0 COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbActuatorType_tbTraslation1_idx` (`tbTranslation_id` ASC)  COMMENT '',
  INDEX `fk_tbActuatorType_tbIcon1_idx` (`tbIcon_id` ASC)  COMMENT '',
  INDEX `fk_tbActuatorType_tbColor1_idx` (`tbColor_id` ASC)  COMMENT '',
  INDEX `fk_tbActuatorType_tbOutput1_idx` (`tbOutput_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbActuatorType_tbColor1`
    FOREIGN KEY (`tbColor_id`)
    REFERENCES `dh`.`tbcolor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbActuatorType_tbIcon1`
    FOREIGN KEY (`tbIcon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbActuatorType_tbOutput1`
    FOREIGN KEY (`tbOutput_id`)
    REFERENCES `dh`.`tboutput` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbActuatorType_tbTraslation1`
    FOREIGN KEY (`tbTranslation_id`)
    REFERENCES `dh`.`tbtranslation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 11
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbnodetype`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbnodetype` (
  `id` INT(11) NOT NULL COMMENT '',
  `tbTranslation_id` INT(11) NOT NULL COMMENT '',
  `tbIcon_id` INT(11) NOT NULL COMMENT '',
  `tbColor_id` INT(11) NOT NULL COMMENT '',
  `scalemin` INT(11) NULL DEFAULT '0' COMMENT '',
  `scalemax` INT(11) NULL DEFAULT '100' COMMENT '',
  `note` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NULL DEFAULT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbNodeType_tbIcon1_idx` (`tbIcon_id` ASC)  COMMENT '',
  INDEX `fk_tbNodeType_tbColor1_idx` (`tbColor_id` ASC)  COMMENT '',
  INDEX `fk_tbNodeType_tbTraslation1_idx` (`tbTranslation_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbNodeType_tbColor1`
    FOREIGN KEY (`tbColor_id`)
    REFERENCES `dh`.`tbcolor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbNodeType_tbIcon1`
    FOREIGN KEY (`tbIcon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbNodeType_tbTraslation1`
    FOREIGN KEY (`tbTranslation_id`)
    REFERENCES `dh`.`tbtranslation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`users`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`users` (
  `user_id` INT(11) NOT NULL AUTO_INCREMENT COMMENT 'auto incrementing user_id of each user, unique index',
  `user_name` VARCHAR(64) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL COMMENT 'user\'s name, unique',
  `user_password_hash` VARCHAR(255) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL COMMENT 'user\'s password in salted and hashed format',
  `user_email` VARCHAR(64) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL COMMENT 'user\'s email, unique',
  `user_active` TINYINT(1) NOT NULL DEFAULT '0' COMMENT 'user\'s activation status',
  `user_activation_hash` VARCHAR(40) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL COMMENT 'user\'s email verification hash string',
  `user_password_reset_hash` CHAR(40) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL COMMENT 'user\'s password reset code',
  `user_password_reset_timestamp` BIGINT(20) NULL DEFAULT NULL COMMENT 'timestamp of the password reset request',
  `user_rememberme_token` VARCHAR(64) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL COMMENT 'user\'s remember-me cookie token',
  `user_failed_logins` TINYINT(1) NOT NULL DEFAULT '0' COMMENT 'user\'s failed login attemps',
  `user_last_failed_login` INT(10) NULL DEFAULT NULL COMMENT 'unix timestamp of last failed login attempt',
  `user_registration_datetime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '',
  `user_registration_ip` VARCHAR(39) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL DEFAULT '0.0.0.0' COMMENT '',
  `userscol` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL COMMENT '',
  `user_language` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL DEFAULT 'EN' COMMENT '',
  PRIMARY KEY (`user_id`)  COMMENT '',
  UNIQUE INDEX `user_name` (`user_name` ASC)  COMMENT '',
  UNIQUE INDEX `user_email` (`user_email` ASC)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 2
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci
COMMENT = 'user data';


-- -----------------------------------------------------
-- Table `dh`.`tblocation`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tblocation` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `users_user_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbLocation_users1_idx` (`users_user_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbLocation_users1`
    FOREIGN KEY (`users_user_id`)
    REFERENCES `dh`.`users` (`user_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 2
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbroom`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbroom` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `tbLocation_id` INT(11) NOT NULL COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `tbicon_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbRoom_tbLocation1_idx` (`tbLocation_id` ASC)  COMMENT '',
  INDEX `fk_tbroom_tbicon1_idx` (`tbicon_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbRoom_tbLocation1`
    FOREIGN KEY (`tbLocation_id`)
    REFERENCES `dh`.`tblocation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbroom_tbicon1`
    FOREIGN KEY (`tbicon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 2
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbstatus`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbstatus` (
  `id` INT(11) NOT NULL COMMENT '',
  `tbIcon_id` INT(11) NOT NULL COMMENT '',
  `tbTranslation_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbStatus_tbIcon1_idx` (`tbIcon_id` ASC)  COMMENT '',
  INDEX `fk_tbStatus_tbTraslation1_idx` (`tbTranslation_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbStatus_tbIcon1`
    FOREIGN KEY (`tbIcon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbStatus_tbTraslation1`
    FOREIGN KEY (`tbTranslation_id`)
    REFERENCES `dh`.`tbtranslation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbnode`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbnode` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `tbNodeType_id` INT(11) NOT NULL COMMENT '',
  `tbStatus_id` INT(11) NOT NULL COMMENT '',
  `xbee_low_address` INT(11) NULL COMMENT '',
  `xbee_high_address` INT(11) NULL COMMENT '',
  `signalvalue` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `currentstatus` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `lastupdate` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '',
  `tbRoom_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbNode_tbNodeType1_idx` (`tbNodeType_id` ASC)  COMMENT '',
  INDEX `fk_tbNode_tbStatus1_idx` (`tbStatus_id` ASC)  COMMENT '',
  INDEX `fk_tbNode_tbStatus2_idx` (`currentstatus` ASC)  COMMENT '',
  INDEX `fk_tbNode_tbRoom1_idx` (`tbRoom_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbNode_tbNodeType1`
    FOREIGN KEY (`tbNodeType_id`)
    REFERENCES `dh`.`tbnodetype` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbNode_tbRoom1`
    FOREIGN KEY (`tbRoom_id`)
    REFERENCES `dh`.`tbroom` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbNode_tbStatus1`
    FOREIGN KEY (`tbStatus_id`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbNode_tbStatus2`
    FOREIGN KEY (`currentstatus`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 2
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbactuator`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbactuator` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `tbActuatorType_id` INT(11) NOT NULL COMMENT '',
  `tbNode_id` INT(11) NOT NULL COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `pinnumber` INT(11) NOT NULL COMMENT '',
  `currentstatus` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `currentvalue` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `fadingtime` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `temptime` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbActuators_tbActuatorType1_idx` (`tbActuatorType_id` ASC)  COMMENT '',
  INDEX `fk_tbActuators_tbNode1_idx` (`tbNode_id` ASC)  COMMENT '',
  INDEX `fk_tbActuators_tbStatus1_idx` (`currentstatus` ASC)  COMMENT '',
  CONSTRAINT `fk_tbActuators_tbActuatorType1`
    FOREIGN KEY (`tbActuatorType_id`)
    REFERENCES `dh`.`tbactuatortype` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbActuators_tbNode1`
    FOREIGN KEY (`tbNode_id`)
    REFERENCES `dh`.`tbnode` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbActuators_tbStatus1`
    FOREIGN KEY (`currentstatus`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 4
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbactionitem`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbactionitem` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `tbActionGroup_id` INT(11) NOT NULL COMMENT '',
  `tbActuators_id` INT(11) NOT NULL COMMENT '',
  `actiontrue` INT(11) NOT NULL COMMENT '',
  `actionfalse` INT(11) NOT NULL COMMENT '',
  `sequence` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbActionSequence_tbActuators1_idx` (`tbActuators_id` ASC)  COMMENT '',
  INDEX `fk_tbActionSequence_tbActionMethod1_idx` (`actiontrue` ASC)  COMMENT '',
  INDEX `fk_tbActionSequence_tbActionMethod2_idx` (`actionfalse` ASC)  COMMENT '',
  INDEX `fk_tbAction_tbActionGroup1_idx` (`tbActionGroup_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbActionSequence_tbActionFalse`
    FOREIGN KEY (`actionfalse`)
    REFERENCES `dh`.`tbaction` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbActionSequence_tbActionTrue`
    FOREIGN KEY (`actiontrue`)
    REFERENCES `dh`.`tbaction` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbActionSequence_tbActuators1`
    FOREIGN KEY (`tbActuators_id`)
    REFERENCES `dh`.`tbactuator` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbAction_tbActionGroup1`
    FOREIGN KEY (`tbActionGroup_id`)
    REFERENCES `dh`.`tbactiongroup` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbactionitem_tbactuator1`
    FOREIGN KEY (`tbActuators_id`)
    REFERENCES `dh`.`tbactuator` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 4
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbchart`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbchart` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `tbTranslation_id` INT(11) NOT NULL COMMENT '',
  `tbicon_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbChart_tbTraslation1_idx` (`tbTranslation_id` ASC)  COMMENT '',
  INDEX `fk_tbchart_tbicon1_idx` (`tbicon_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbChart_tbTraslation1`
    FOREIGN KEY (`tbTranslation_id`)
    REFERENCES `dh`.`tbtranslation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbchart_tbicon1`
    FOREIGN KEY (`tbicon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 4
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbconvformula`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbconvformula` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `formula` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 6
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbcustomparams`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbcustomparams` (
  `id` INT(11) NOT NULL COMMENT '',
  `idx` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `tbTraslation_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbCustomParams_tbTraslation1_idx` (`tbTraslation_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbCustomParams_tbTraslation1`
    FOREIGN KEY (`tbTraslation_id`)
    REFERENCES `dh`.`tbtranslation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbdatain`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbdatain` (
  `timekey` BIGINT(20) NOT NULL COMMENT '',
  `type` INT(11) NOT NULL COMMENT '0=Node 1=Sensor 2=Actuator',
  `v0` INT(11) NOT NULL COMMENT '',
  `v1` INT(11) NULL DEFAULT NULL COMMENT '',
  `v2` INT(11) NULL DEFAULT NULL COMMENT '',
  `v3` INT(11) NULL DEFAULT NULL COMMENT '',
  `v4` INT(11) NULL DEFAULT NULL COMMENT '',
  `v5` INT(11) NULL DEFAULT NULL COMMENT '',
  `v6` INT(11) NULL DEFAULT NULL COMMENT '',
  `v7` INT(11) NULL DEFAULT NULL COMMENT '',
  `v8` INT(11) NULL DEFAULT NULL COMMENT '',
  `V9` INT(11) NULL DEFAULT NULL COMMENT '',
  `V10` INT(11) NULL DEFAULT NULL COMMENT '')
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbdataout`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbdataout` (
  `timekey` BIGINT(20) NOT NULL COMMENT '',
  `type` INT NOT NULL COMMENT '0=set trigger\n1=delete local trigger \n2=set actuator \n3=reset remote trigger\n4=set remote trigger\n5=delete remote trigger',
  `v0` INT NOT NULL COMMENT '',
  `v1` INT NULL DEFAULT NULL COMMENT '',
  `v2` INT NULL DEFAULT NULL COMMENT '',
  `v3` INT NULL DEFAULT NULL COMMENT '',
  `v4` INT NULL DEFAULT NULL COMMENT '',
  `v5` INT NULL DEFAULT NULL COMMENT '',
  `v6` INT NULL DEFAULT NULL COMMENT '',
  `V7` INT NULL DEFAULT NULL COMMENT '',
  `v8` INT NULL DEFAULT NULL COMMENT '',
  `V9` INT NULL DEFAULT NULL COMMENT '',
  `V10` INT NULL DEFAULT NULL COMMENT '')
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbevent`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbevent` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL DEFAULT 'type event name here' COMMENT '',
  `currentstatus` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 3
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbparam`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbparam` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `pvalue` INT(11) NOT NULL COMMENT '',
  `descr` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `ptype` VARCHAR(1) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `pindex` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 9
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbum`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbum` (
  `id` INT NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(3) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `dh`.`tbsensortype`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbsensortype` (
  `id` INT(11) NOT NULL COMMENT '',
  `tbTranslation_id` INT(11) NOT NULL COMMENT '',
  `tbConvFormula_id` INT(11) NOT NULL COMMENT '',
  `tbIcon_id` INT(11) NOT NULL COMMENT '',
  `tbColor_id` INT(11) NOT NULL COMMENT '',
  `scalemin` INT(11) NOT NULL COMMENT '',
  `scalemax` INT(11) NOT NULL COMMENT '',
  `timer` INT(11) NOT NULL DEFAULT '1' COMMENT 'time interval to save sensor current value to historical data for charts in dashboard.',
  `notes` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NULL DEFAULT NULL COMMENT '',
  `tbum_id` INT NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbSensorType_tbConvFormula1_idx` (`tbConvFormula_id` ASC)  COMMENT '',
  INDEX `fk_tbSensorType_tbParams1_idx` (`timer` ASC)  COMMENT '',
  INDEX `fk_tbSensorType_tbIcon1_idx` (`tbIcon_id` ASC)  COMMENT '',
  INDEX `fk_tbSensorType_tbColor1_idx` (`tbColor_id` ASC)  COMMENT '',
  INDEX `fk_tbSensorType_tbTraslation1_idx` (`tbTranslation_id` ASC)  COMMENT '',
  INDEX `fk_tbsensortype_tbum1_idx` (`tbum_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbSensorType_tbColor1`
    FOREIGN KEY (`tbColor_id`)
    REFERENCES `dh`.`tbcolor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSensorType_tbConvFormula1`
    FOREIGN KEY (`tbConvFormula_id`)
    REFERENCES `dh`.`tbconvformula` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSensorType_tbIcon1`
    FOREIGN KEY (`tbIcon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSensorType_tbParams1`
    FOREIGN KEY (`timer`)
    REFERENCES `dh`.`tbparam` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSensorType_tbTraslation1`
    FOREIGN KEY (`tbTranslation_id`)
    REFERENCES `dh`.`tbtranslation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbsensortype_tbum1`
    FOREIGN KEY (`tbum_id`)
    REFERENCES `dh`.`tbum` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbsensor`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbsensor` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `tbNode_id` INT(11) NOT NULL COMMENT '',
  `tbStatus_id` INT(11) NOT NULL COMMENT '',
  `tbSensorType_id` INT(11) NOT NULL COMMENT '',
  `pin_number` INT(11) NOT NULL COMMENT '',
  `currentvalue` FLOAT NOT NULL DEFAULT '0' COMMENT '',
  `addtohomepage` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `tbChart_id` INT(11) NULL DEFAULT NULL COMMENT '',
  `lastupdate` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '',
  `currentstatus` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  UNIQUE INDEX `id_UNIQUE` (`id` ASC)  COMMENT '',
  INDEX `fk_tbSensor_tbStatus1_idx` (`tbStatus_id` ASC)  COMMENT '',
  INDEX `fk_tbSensor_tbSensorType1_idx` (`tbSensorType_id` ASC)  COMMENT '',
  INDEX `fk_tbSensor_tbNode1_idx` (`tbNode_id` ASC)  COMMENT '',
  INDEX `fk_tbSensor_tbChart1_idx` (`tbChart_id` ASC)  COMMENT '',
  INDEX `fk_tbSensor_tbStatus2_idx` (`currentstatus` ASC)  COMMENT '',
  CONSTRAINT `fk_tbSensor_tbChart1`
    FOREIGN KEY (`tbChart_id`)
    REFERENCES `dh`.`tbchart` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSensor_tbNode1`
    FOREIGN KEY (`tbNode_id`)
    REFERENCES `dh`.`tbnode` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSensor_tbSensorType1`
    FOREIGN KEY (`tbSensorType_id`)
    REFERENCES `dh`.`tbsensortype` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSensor_tbStatus1`
    FOREIGN KEY (`tbStatus_id`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSensor_tbStatusCurrent`
    FOREIGN KEY (`currentstatus`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 5
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbeventlogic`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbeventlogic` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `tbEvent_id` INT(11) NOT NULL COMMENT '',
  `tbSensor_id` INT(11) NOT NULL COMMENT '',
  `valmin` FLOAT NOT NULL DEFAULT '0' COMMENT '',
  `valmax` FLOAT NOT NULL DEFAULT '1023' COMMENT '',
  `sequence` INT(11) NOT NULL DEFAULT '1' COMMENT '',
  `logic` INT(11) NULL DEFAULT NULL COMMENT 'logic = 0=AND 1=OR NULL=end logic group',
  `bracket` VARCHAR(1) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NULL DEFAULT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbEventLogic_tbEvent1_idx` (`tbEvent_id` ASC)  COMMENT '',
  INDEX `fk_tbEventLogic_tbSensor1_idx` (`tbSensor_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbEventLogic_tbEvent1`
    FOREIGN KEY (`tbEvent_id`)
    REFERENCES `dh`.`tbevent` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbEventLogic_tbSensor1`
    FOREIGN KEY (`tbSensor_id`)
    REFERENCES `dh`.`tbsensor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 5
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbhistval`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbhistval` (
  `tbSensor_id` INT(11) NOT NULL COMMENT '',
  `value` FLOAT NOT NULL COMMENT '',
  `date` DATETIME NOT NULL COMMENT '',
  INDEX `fk_tbHistval_tbSensor1_idx` (`tbSensor_id` ASC)  COMMENT '',
  INDEX `idx_sensor_date` USING BTREE (`date` ASC, `tbSensor_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbHistval_tbSensor1`
    FOREIGN KEY (`tbSensor_id`)
    REFERENCES `dh`.`tbsensor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tblanguage`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tblanguage` (
  `id` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `descr` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbsendemail`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbsendemail` (
  `currentval` INT(11) NOT NULL COMMENT '',
  `date` DATETIME NOT NULL COMMENT '',
  `tbSensor_id` INT(11) NOT NULL COMMENT '',
  `tbEvent_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`currentval`)  COMMENT '',
  INDEX `fk_tbSendEmail_tbSensor1_idx` (`tbSensor_id` ASC)  COMMENT '',
  INDEX `fk_tbSendEmail_tbEvent1_idx` (`tbEvent_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbSendEmail_tbEvent1`
    FOREIGN KEY (`tbEvent_id`)
    REFERENCES `dh`.`tbevent` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSendEmail_tbSensor1`
    FOREIGN KEY (`tbSensor_id`)
    REFERENCES `dh`.`tbsensor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbsystem`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbsystem` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `users_user_id` INT(11) NOT NULL COMMENT '',
  `tbstatus_id` INT(11) NOT NULL COMMENT '',
  `tbicon_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbSystem_users1_idx` (`users_user_id` ASC)  COMMENT '',
  INDEX `fk_tbsystem_tbstatus1_idx` (`tbstatus_id` ASC)  COMMENT '',
  INDEX `fk_tbsystem_tbicon1_idx` (`tbicon_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbSystem_users1`
    FOREIGN KEY (`users_user_id`)
    REFERENCES `dh`.`users` (`user_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbsystem_tbstatus1`
    FOREIGN KEY (`tbstatus_id`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbsystem_tbicon1`
    FOREIGN KEY (`tbicon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 3
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbsubsystem`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbsubsystem` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `tbSystem_id` INT(11) NOT NULL COMMENT '',
  `tbicon_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbSubsystem_tbSystem1_idx` (`tbSystem_id` ASC)  COMMENT '',
  INDEX `fk_tbsubsystem_tbicon1_idx` (`tbicon_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbSubsystem_tbSystem1`
    FOREIGN KEY (`tbSystem_id`)
    REFERENCES `dh`.`tbsystem` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbsubsystem_tbicon1`
    FOREIGN KEY (`tbicon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 3
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbsubsystemactuator`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbsubsystemactuator` (
  `tbSubsystem_id` INT(11) NOT NULL COMMENT '',
  `tbActuators_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`tbSubsystem_id`, `tbActuators_id`)  COMMENT '',
  INDEX `fk_tbSubsystemActuator_tbSubsystem1_idx` (`tbSubsystem_id` ASC)  COMMENT '',
  INDEX `fk_tbSubsystemActuator_tbActuators1_idx` (`tbActuators_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbSubsystemActuator_tbActuators1`
    FOREIGN KEY (`tbActuators_id`)
    REFERENCES `dh`.`tbactuator` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbSubsystemActuator_tbSubsystem1`
    FOREIGN KEY (`tbSubsystem_id`)
    REFERENCES `dh`.`tbsubsystem` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbsubsystemsensor`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbsubsystemsensor` (
  `tbSubsystem_id` INT(11) NOT NULL COMMENT '',
  `tbsensor_id` INT(11) NOT NULL COMMENT '',
  `tbstatus_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`tbSubsystem_id`, `tbsensor_id`)  COMMENT '',
  INDEX `fk_tbSubsystemTrigger_tbSubsystem1_idx` (`tbSubsystem_id` ASC)  COMMENT '',
  INDEX `fk_tbsubsystemtrigger_tbstatus1_idx` (`tbstatus_id` ASC)  COMMENT '',
  INDEX `fk_tbsubsystemtrigger_tbsensor1_idx` (`tbsensor_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbSubsystemTrigger_tbSubsystem1`
    FOREIGN KEY (`tbSubsystem_id`)
    REFERENCES `dh`.`tbsubsystem` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbsubsystemtrigger_tbstatus1`
    FOREIGN KEY (`tbstatus_id`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbsubsystemtrigger_tbsensor1`
    FOREIGN KEY (`tbsensor_id`)
    REFERENCES `dh`.`tbsensor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbsubsystemsensor`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbsubsystemsensor` (
  `tbSubsystem_id` INT(11) NOT NULL COMMENT '',
  `tbsensor_id` INT(11) NOT NULL COMMENT '',
  `tbstatus_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`tbSubsystem_id`, `tbsensor_id`)  COMMENT '',
  INDEX `fk_tbSubsystemTrigger_tbSubsystem1_idx` (`tbSubsystem_id` ASC)  COMMENT '',
  INDEX `fk_tbsubsystemtrigger_tbstatus1_idx` (`tbstatus_id` ASC)  COMMENT '',
  INDEX `fk_tbsubsystemtrigger_tbsensor1_idx` (`tbsensor_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbSubsystemTrigger_tbSubsystem1`
    FOREIGN KEY (`tbSubsystem_id`)
    REFERENCES `dh`.`tbsubsystem` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbsubsystemtrigger_tbstatus1`
    FOREIGN KEY (`tbstatus_id`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbsubsystemtrigger_tbsensor1`
    FOREIGN KEY (`tbsensor_id`)
    REFERENCES `dh`.`tbsensor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbtimerdaily`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbtimerdaily` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `timestart` TIME NOT NULL DEFAULT '00:00:00' COMMENT '',
  `timeend` TIME NOT NULL DEFAULT '00:00:24' COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 3
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbtimermonthly`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbtimermonthly` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `m1` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m2` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m3` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m4` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m5` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m6` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m7` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m8` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m9` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m10` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m11` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `m12` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 4
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbtimerweekly`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbtimerweekly` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `wd0` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `wd1` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `wd2` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `wd3` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `wd4` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `wd5` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  `wd6` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 4
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbtimer`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbtimer` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `tbTimerDaily_id` INT(11) NOT NULL COMMENT '',
  `tbTimerMonthly_id` INT(11) NOT NULL COMMENT '',
  `tbTimerWeekly_id` INT(11) NOT NULL COMMENT '',
  `currentstatus` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbTimer_tbTimerDaily1_idx` (`tbTimerDaily_id` ASC)  COMMENT '',
  INDEX `fk_tbTimer_tbTimerMonthly1_idx` (`tbTimerMonthly_id` ASC)  COMMENT '',
  INDEX `fk_tbTimer_tbTimerWeekly1_idx` (`tbTimerWeekly_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbTimer_tbTimerDaily1`
    FOREIGN KEY (`tbTimerDaily_id`)
    REFERENCES `dh`.`tbtimerdaily` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbTimer_tbTimerMonthly1`
    FOREIGN KEY (`tbTimerMonthly_id`)
    REFERENCES `dh`.`tbtimermonthly` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbTimer_tbTimerWeekly1`
    FOREIGN KEY (`tbTimerWeekly_id`)
    REFERENCES `dh`.`tbtimerweekly` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 4
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbtranslationitem`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbtranslationitem` (
  `tbLanguage_id` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  `tbTranslation_id` INT(11) NOT NULL COMMENT '',
  `wording` VARCHAR(200) CHARACTER SET 'utf8' COLLATE 'utf8_bin' NOT NULL COMMENT '',
  PRIMARY KEY (`tbLanguage_id`, `tbTranslation_id`)  COMMENT '',
  INDEX `fk_tbTranslationItem_tbTraslation1_idx` (`tbTranslation_id` ASC)  COMMENT '',
  INDEX `fk_tbTranslationItem_tbLanguage1_idx` (`tbLanguage_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbTranslationItem_tbLanguage1`
    FOREIGN KEY (`tbLanguage_id`)
    REFERENCES `dh`.`tblanguage` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbTranslationItem_tbTraslation1`
    FOREIGN KEY (`tbTranslation_id`)
    REFERENCES `dh`.`tbtranslation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbtriggertype`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbtriggertype` (
  `id` INT(11) NOT NULL COMMENT '',
  `tbIcon_id` INT(11) NOT NULL COMMENT '',
  `tbTranslation_id` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbTriggerType_tbIcon1_idx` (`tbIcon_id` ASC)  COMMENT '',
  INDEX `fk_tbTriggerType_tbTraslation1_idx` (`tbTranslation_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbTriggerType_tbIcon1`
    FOREIGN KEY (`tbIcon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbTriggerType_tbTraslation1`
    FOREIGN KEY (`tbTranslation_id`)
    REFERENCES `dh`.`tbtranslation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbtrigger`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbtrigger` (
  `id` INT(11) NOT NULL COMMENT '',
  `tbTriggerType_id` INT(11) NOT NULL COMMENT '',
  `tbEvent_id` INT(11) NOT NULL COMMENT '',
  `tbActionGroup_id` INT(11) NOT NULL COMMENT '',
  `tbStatus_id` INT(11) NOT NULL COMMENT '',
  `tbTimer_id` INT(11) NOT NULL COMMENT '',
  `currentstatus` INT(11) NOT NULL DEFAULT '0' COMMENT '',
  `sendalert` INT NOT NULL DEFAULT 0 COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbTrigger_tbEvent1_idx` (`tbEvent_id` ASC)  COMMENT '',
  INDEX `fk_tbTrigger_tbTriggerType1_idx` (`tbTriggerType_id` ASC)  COMMENT '',
  INDEX `fk_tbTrigger_tbStatus1_idx` (`tbStatus_id` ASC)  COMMENT '',
  INDEX `fk_tbTrigger_tbTimer1_idx` (`tbTimer_id` ASC)  COMMENT '',
  INDEX `fk_tbTrigger_tbActionGroup1_idx` (`tbActionGroup_id` ASC)  COMMENT '',
  UNIQUE INDEX `tbActionGroup_id_UNIQUE` (`tbActionGroup_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbTrigger_tbActionGroup1`
    FOREIGN KEY (`tbActionGroup_id`)
    REFERENCES `dh`.`tbactiongroup` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbTrigger_tbEvent1`
    FOREIGN KEY (`tbEvent_id`)
    REFERENCES `dh`.`tbevent` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbTrigger_tbStatus1`
    FOREIGN KEY (`tbStatus_id`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbTrigger_tbTimer1`
    FOREIGN KEY (`tbTimer_id`)
    REFERENCES `dh`.`tbtimer` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbTrigger_tbTriggerType1`
    FOREIGN KEY (`tbTriggerType_id`)
    REFERENCES `dh`.`tbtriggertype` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;


-- -----------------------------------------------------
-- Table `dh`.`tbpopup`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbpopup` (
  `tbtrigger_id` INT(11) NOT NULL COMMENT '',
  `tbsensor_id` INT(11) NOT NULL COMMENT '',
  `cwhy` VARCHAR(500) NULL COMMENT '',
  `cwhen` VARCHAR(500) NULL COMMENT '',
  `cwhat` VARCHAR(500) NULL COMMENT '',
  INDEX `fk_tbPopup_tbsensor1_idx` (`tbsensor_id` ASC)  COMMENT '',
  INDEX `fk_tbPopup_tbtrigger1_idx` (`tbtrigger_id` ASC)  COMMENT '',
  PRIMARY KEY (`tbtrigger_id`, `tbsensor_id`)  COMMENT '',
  CONSTRAINT `fk_tbPopup_tbsensor1`
    FOREIGN KEY (`tbsensor_id`)
    REFERENCES `dh`.`tbsensor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbPopup_tbtrigger1`
    FOREIGN KEY (`tbtrigger_id`)
    REFERENCES `dh`.`tbtrigger` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `dh`.`tblog`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tblog` (
  `type` VARCHAR(2) NOT NULL COMMENT '',
  `msg` VARCHAR(500) NOT NULL COMMENT '',
  `data` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP COMMENT '')
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `dh`.`tbproctl`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbproctl` (
  `id` INT NOT NULL COMMENT '',
  `sts` INT NOT NULL DEFAULT 0 COMMENT '',
  `proc` VARCHAR(45) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `dh`.`tbfunction`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbfunction` (
  `id` INT NOT NULL AUTO_INCREMENT COMMENT '',
  `descr` VARCHAR(100) NOT NULL COMMENT '',
  `tbicon_id` INT(11) NOT NULL COMMENT '',
  `currentstatus` INT(11) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '',
  INDEX `fk_tbfunction_tbicon1_idx` (`tbicon_id` ASC)  COMMENT '',
  INDEX `fk_tbfunction_tbstatus1_idx` (`currentstatus` ASC)  COMMENT '',
  CONSTRAINT `fk_tbfunction_tbicon1`
    FOREIGN KEY (`tbicon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbfunction_tbstatus1`
    FOREIGN KEY (`currentstatus`)
    REFERENCES `dh`.`tbstatus` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `dh`.`tbfunctiontrigger`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbfunctiontrigger` (
  `tbfunction_id` INT NOT NULL COMMENT '',
  `tbtrigger_id` INT(11) NOT NULL COMMENT '',
  INDEX `fk_tbfunctionactiongroup_tbfunction1_idx` (`tbfunction_id` ASC)  COMMENT '',
  INDEX `fk_tbfunctionactiongroup_tbtrigger1_idx` (`tbtrigger_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbfunctionactiongroup_tbfunction1`
    FOREIGN KEY (`tbfunction_id`)
    REFERENCES `dh`.`tbfunction` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbfunctionactiongroup_tbtrigger1`
    FOREIGN KEY (`tbtrigger_id`)
    REFERENCES `dh`.`tbtrigger` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `dh`.`tbmenu`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`tbmenu` (
  `page` VARCHAR(45) NOT NULL COMMENT '',
  `tbicon_id` INT(11) NOT NULL COMMENT '',
  `tbtranslation_id` INT(11) NOT NULL COMMENT '',
  INDEX `fk_tbmenu_tbicon1_idx` (`tbicon_id` ASC)  COMMENT '',
  INDEX `fk_tbmenu_tbtranslation1_idx` (`tbtranslation_id` ASC)  COMMENT '',
  CONSTRAINT `fk_tbmenu_tbicon1`
    FOREIGN KEY (`tbicon_id`)
    REFERENCES `dh`.`tbicon` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tbmenu_tbtranslation1`
    FOREIGN KEY (`tbtranslation_id`)
    REFERENCES `dh`.`tbtranslation` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

USE `dh` ;

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_vwremotetriggers`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_vwremotetriggers` (`trigger_id` INT, `triggergroup_id` INT, `triggeritem_id` INT, `trigger_type` INT, `node_id` INT, `sensor_id` INT, `sensor_pin` INT, `actuator_pin` INT, `valuemin` INT, `valuemax` INT, `output` INT, `actionTrue` INT, `actionFalse` INT, `fadingtime` INT, `temptime` INT, `currentstatus` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_vwtriggers`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_vwtriggers` (`trigger_id` INT, `triggergroup_id` INT, `triggeritem_id` INT, `trigger_type` INT, `node_id` INT, `actuator_pin` INT, `action` INT, `trigger_status` INT, `trigger_currentstatus` INT, `output` INT, `fadingtime` INT, `temptime` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwevents`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwevents` (`id` INT, `descr` INT, `event_status` INT, `node` INT, `tbSensor_id` INT, `sensor` INT, `sensor_type` INT, `valmin` INT, `valmax` INT, `currentvalue` INT, `sensorcurrentstatus` INT, `sequence` INT, `logic` INT, `s_logic` INT, `bracket` INT, `scalemin` INT, `scalemax` INT, `eventlogic_id` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwnodes`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwnodes` (`id` INT, `status` INT, `xbee_low_address` INT, `xbee_high_address` INT, `nodeType` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwsensors`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwsensors` (`Sensor_id` INT, `NodeId` INT, `tbNodeType_id` INT, `tbSensorType_id` INT, `tbStatus_id` INT, `pin_number` INT, `Timer` INT, `currentvalue` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtranslation`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtranslation` (`id` INT, `wording` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtriggers`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtriggers` (`trigger_id` INT, `triggergroup_id` INT, `triggeritem_id` INT, `trigger_status` INT, `trigger_currentstatus` INT, `trigger_type` INT, `event_id` INT, `event_status` INT, `triggerType_id` INT, `actiongroup_id` INT, `sequence` INT, `valueTrue` INT, `valueFalse` INT, `actuator_id` INT, `actuator_name` INT, `actuatorType_id` INT, `actuatorType` INT, `node_id` INT, `actuator_pin` INT, `actuator_status` INT, `fadingtime` INT, `temptime` INT, `output` INT, `timer_id` INT, `timer_name` INT, `timer_status` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwactuator`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwactuator` (`id` INT, `tbActuatorType_id` INT, `tbNode_id` INT, `descr` INT, `pinnumber` INT, `currentstatus` INT, `currentvalue` INT, `tbOutput_id` INT, `valueon` INT, `valueoff` INT, `temptime` INT, `fadingtime` INT, `tbIcon_id` INT, `actuatortypeicon` INT, `actuatortypecolor` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwdashboardsensorshomepage`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwdashboardsensorshomepage` (`id` INT, `tbNode_id` INT, `descr` INT, `pin_number` INT, `currentvalue` INT, `status` INT, `lastupdate` INT, `scalemin` INT, `scalemax` INT, `hexCode` INT, `sensor_type` INT, `um` INT, `addtohomepage` INT, `room` INT, `roomname` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwdashboardactuators`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwdashboardactuators` (`id` INT, `tbActuatorType_id` INT, `tbNode_id` INT, `descr` INT, `pinnumber` INT, `currentstatus` INT, `currentstatusicon` INT, `currentvalue` INT, `status` INT, `tbOutput_id` INT, `valueon` INT, `valueoff` INT, `temptime` INT, `fadingtime` INT, `actuatortypeicon` INT, `actuatortypecolor` INT, `currentstatuscolor` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwcharts`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwcharts` (`id` INT, `wording` INT, `icon` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwnodesdashboard`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwnodesdashboard` (`id` INT, `status` INT, `tipo` INT, `descr` INT, `currentstatus` INT, `scalemin` INT, `scalemax` INT, `color` INT, `signal` INT, `room` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwdashboardsensorsdetails`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwdashboardsensorsdetails` (`id` INT, `tbNode_id` INT, `descr` INT, `pin_number` INT, `currentvalue` INT, `status` INT, `lastupdate` INT, `scalemin` INT, `scalemax` INT, `hexCode` INT, `sensor_type` INT, `currentstatus` INT, `um` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtriggersdashboard`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtriggersdashboard` (`id` INT, `sensor_id` INT, `valuemin` INT, `valuemax` INT, `currentstatus` INT, `fadingtime` INT, `temptime` INT, `tbStatus_id` INT, `trigger_type` INT, `actuator_icon` INT, `currentstatus_icon` INT, `node` INT, `actuator_id` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtriggers`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtriggers` (`trigger_id` INT, `triggergroup_id` INT, `triggeritem_id` INT, `trigger_status` INT, `trigger_currentstatus` INT, `trigger_type` INT, `event_id` INT, `event_status` INT, `triggerType_id` INT, `actiongroup_id` INT, `sequence` INT, `valueTrue` INT, `valueFalse` INT, `actuator_id` INT, `actuator_name` INT, `actuatorType_id` INT, `actuatorType` INT, `node_id` INT, `actuator_pin` INT, `actuator_status` INT, `fadingtime` INT, `temptime` INT, `output` INT, `timer_id` INT, `timer_name` INT, `timer_status` INT, `sensor` INT, `action` INT, `room` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_tbsensortype`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbsensortype` (`id` INT, `wording` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_tbstatus`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbstatus` (`id` INT, `wording` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_tbstatus`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbstatus` (`id` INT, `wording` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_tbtriggertype`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbtriggertype` (`id` INT, `wording` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_tbactuatortype`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbactuatortype` (`id` INT, `wording` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_tbchart`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbchart` (`id` INT, `wording` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_tbnodetype`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbnodetype` (`id` INT, `wording` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtriggersensordetails`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtriggersensordetails` (`id` INT, `sensor_id` INT, `valuemin` INT, `valuemax` INT, `currentstatus` INT, `fadingtime` INT, `temptime` INT, `tbStatus_id` INT, `trigger_type` INT, `currentstatus_icon` INT, `node` INT, `node_name` INT, `action_group` INT, `actuator_pin` INT, `event` INT, `timer` INT, `actuator_icon` INT, `actuator` INT, `actuator_currentstatus` INT, `sequence` INT, `actionon` INT, `actionoff` INT, `actionitem_id` INT, `actiongroup_id` INT, `actuator_id` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtriggertitle`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtriggertitle` (`id` INT, `trigger_type` INT, `currentstatus_icon` INT, `triggerTitle` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtriggertitle`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtriggertitle` (`id` INT, `trigger_type` INT, `currentstatus_icon` INT, `triggerTitle` INT, `timer` INT, `timer_currentstatus` INT, `trigger_status` INT, `sendalert` INT, `actiongroup_id` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwactuatorlist`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwactuatorlist` (`id` INT, `actuator` INT, `node` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwsensorslist`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwsensorslist` (`id` INT, `node` INT, `sensor` INT, `chart` INT, `room` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtriggersall`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtriggersall` (`id` INT, `sensor_id` INT, `valuemin` INT, `valuemax` INT, `currentstatus` INT, `fadingtime` INT, `temptime` INT, `tbStatus_id` INT, `trigger_type` INT, `actuator_icon` INT, `currentstatus_icon` INT, `node` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_vwsystems`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_vwsystems` (`id` INT, `system` INT, `icon` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_vwsubsystems`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_vwsubsystems` (`system_id` INT, `system` INT, `user` INT, `subsystem_id` INT, `subsystem` INT, `descr` INT, `id` INT, `sensor_id` INT, `valuemin` INT, `valuemax` INT, `currentstatus` INT, `fadingtime` INT, `temptime` INT, `tbStatus_id` INT, `trigger_type` INT, `currentstatus_icon` INT, `node` INT, `node_name` INT, `action_group` INT, `actuator_pin` INT, `event` INT, `timer` INT, `actuator_icon` INT, `actuator` INT, `actuator_currentstatus` INT, `sequence` INT, `actionon` INT, `actionoff` INT, `actionitem_id` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_tbmenu`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_tbmenu` (`page` INT, `icon` INT, `menuitem` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_vwsubsystems`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_vwsubsystems` (`system_id` INT, `system` INT, `user` INT, `subsystem_id` INT, `subsystem` INT, `sensor_id` INT, `sensor` INT, `id` INT, `valuemin` INT, `valuemax` INT, `currentstatus` INT, `fadingtime` INT, `temptime` INT, `tbStatus_id` INT, `trigger_type` INT, `currentstatus_icon` INT, `node` INT, `node_name` INT, `action_group` INT, `actuator_pin` INT, `event` INT, `timer` INT, `actuator_icon` INT, `actuator` INT, `actuator_currentstatus` INT, `sequence` INT, `actionon` INT, `actionoff` INT, `actionitem_id` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwsubsystemactuators`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwsubsystemactuators` (`subsystem_id` INT, `sensor_id` INT, `id` INT, `tbActuatorType_id` INT, `tbNode_id` INT, `descr` INT, `pinnumber` INT, `currentstatus` INT, `currentstatusicon` INT, `currentvalue` INT, `status` INT, `tbOutput_id` INT, `valueon` INT, `valueoff` INT, `temptime` INT, `fadingtime` INT, `actuatortypeicon` INT, `actuatortypecolor` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`_vwfunctions`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`_vwfunctions` (`id` INT, `function` INT, `icon` INT, `currentstatusicon` INT, `currentstatus` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtriggerfunctiontitle`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtriggerfunctiontitle` (`id` INT, `trigger_type` INT, `currentstatus_icon` INT, `triggerTitle` INT, `timer` INT, `timer_currentstatus` INT, `trigger_status` INT, `sendalert` INT, `actiongroup_id` INT, `actiongroup` INT, `function_id` INT, `function` INT, `function_currentstatus` INT, `event` INT, `event_status` INT);

-- -----------------------------------------------------
-- Placeholder table for view `dh`.`vwtriggerfunctionall`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `dh`.`vwtriggerfunctionall` (`id` INT, `sensor_id` INT, `valuemin` INT, `valuemax` INT, `currentstatus` INT, `fadingtime` INT, `temptime` INT, `tbStatus_id` INT, `trigger_type` INT, `actuator_icon` INT, `currentstatus_icon` INT, `node` INT, `function_id` INT, `room` INT, `roomtitle` INT, `triggertitle` INT);

-- -----------------------------------------------------
-- function f_GA1A12S202
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `f_GA1A12S202`(pval INT, pmode INT) RETURNS float
    DETERMINISTIC
BEGIN

DECLARE logRange FLOAT;
DECLARE rawRange FLOAT;
DECLARE logLux FLOAT;

IF pmode = 0 THEN 
	SET logRange = 5.0;
	SET rawRange = 1023;
	SET logLux = pval * logRange / rawRange;
	return floor(pow(10, logLux));
END IF;

IF pmode = 1 THEN 
	SET logRange = 5.0;
	SET rawRange = 1023;
	SET logLux = log(10, pval);
   return floor(logLux * rawRange / logRange);
END IF;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function f_analoglux
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` FUNCTION `f_analoglux`(pval INT, pmode INT) RETURNS float
    DETERMINISTIC
BEGIN

IF pmode = 0 THEN 
	return floor(pval/10);	
END IF;

IF pmode = 1 THEN 
	return floor(pval*10);	
END IF;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function f_dht21
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `f_dht21`(pval INT, pmode INT) RETURNS float
    DETERMINISTIC
BEGIN

IF pmode = 0 THEN 
	return pval/100;
END IF;

IF pmode = 1 THEN 
	return FLOOR(pval*100);
END IF;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function f_generic
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `f_generic`(pval INT, pmode INT) RETURNS float
    DETERMINISTIC
BEGIN

return pval;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function f_temp
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `f_temp`(pval INT, pmode INT) RETURNS float
    DETERMINISTIC
BEGIN

DECLARE resist FLOAT;
DECLARE stdres INT;
DECLARE k FLOAT;
DECLARE h FLOAT;
DECLARE w FLOAT;
DECLARE T FLOAT;
DECLARE Z FLOAT;

IF pmode = 0 THEN 
	SET stdres = 10000;
	SET resist = stdres*((1024/pval)-1);
	SET k = 1/298.15;
	SET h = 1/3975;
	SET w = log(resist/stdres);
	SET T=1/(k+(h*w));
	Return T-273.15;
END IF;

IF pmode = 1 THEN 
	SET stdres = 10000;
	
	SET k = 1/298.15;
	SET h = 1/3975;
	
	SET T = pval + 273.15;
	SET Z = ((1/T)-k)/h;
	SET resist = exp(Z)*10000;
	return floor(1024/((resist/stdres)+1));
END IF;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function generatesqlchartseries
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `generatesqlchartseries`(pval INT) RETURNS varchar(2000) CHARSET utf8
    DETERMINISTIC
BEGIN

SET NAMES 'utf8';

SELECT
  GROUP_CONCAT(DISTINCT
    CONCAT(
      ' MAX(CASE WHEN tbhistval.tbSensor_id = '
	,  tbhistval.tbsensor_id 
	, ' THEN tbhistval.value END) ''' 
	,  tbsensor.id 
	, '''' )
  ) INTO @sql
FROM tbhistval, tbsensor
WHERE tbhistval.tbsensor_id = tbsensor.id
AND tbsensor.tbchart_id = pval;
SET @sql = CONCAT('SELECT DATE_FORMAT(tbhistval.date,'
,'''%Y/%m/%d %H:%i:00'''
,') as Data ,'
, @sql 
,'FROM tbhistval, tbsensor WHERE tbhistval.tbsensor_id = tbsensor.id and tbhistval.date between DATE_SUB(NOW(), INTERVAL 1 DAY) AND NOW() AND tbsensor.tbchart_id = '''
, pval 
, ''''
,' GROUP BY DATE_FORMAT(tbhistval.date,'
,'''%d%m%Y%H%i'''
,') ORDER BY DATE_FORMAT(tbhistval.date,' 
,'''%d%m%Y%H%i'''
,')'
);
return @sql;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function generatesqlchartseriesSensor
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `generatesqlchartseriesSensor`(pval INT) RETURNS varchar(2000) CHARSET latin1
    DETERMINISTIC
BEGIN

SET NAMES 'utf8';

SELECT
  GROUP_CONCAT(DISTINCT
    CONCAT(
      ' MAX(CASE WHEN tbhistval.tbsensor_id = '
	,  tbhistval.tbsensor_id 
	, ' THEN tbhistval.value END) ''' 
	,  tbsensor.descr 
	, '''' )
  ) INTO @sql
FROM tbhistval, tbsensor
WHERE tbhistval.tbsensor_id = tbsensor.id
AND tbsensor.id = pval;
SET @sql = CONCAT('SELECT DATE_FORMAT(tbhistval.date,'
,'''%Y/%m/%d %H:%i:00'''
,') as Data ,'
, @sql 
,'FROM tbhistval, tbsensor WHERE tbhistval.tbsensor_id = tbsensor.id and tbhistval.date between DATE_SUB(NOW(), INTERVAL 1 DAY) AND NOW() AND tbsensor.id = '
, pval 
,' GROUP BY DATE_FORMAT(tbhistval.date,'
,'''%d%m%Y%H%i'''
,') ORDER BY DATE_FORMAT(tbhistval.date,' 
,'''%d%m%Y%H%i'''
,')'
);
return @sql;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function generatesqlchartseriesheaders
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `generatesqlchartseriesheaders`(pval INT) RETURNS varchar(2000) CHARSET latin1
    DETERMINISTIC
BEGIN

SET NAMES 'utf8';

SELECT
  GROUP_CONCAT(DISTINCT
    CONCAT( tbsensor.id )
  ) INTO @sql
FROM tbhistval, tbsensor
WHERE tbhistval.tbsensor_id = tbsensor.id
AND tbsensor.tbchart_id = pval;

return @sql;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function generatesqlchartseriesheadersSensor
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `generatesqlchartseriesheadersSensor`(pval INT) RETURNS varchar(2000) CHARSET latin1
    DETERMINISTIC
BEGIN

SET NAMES 'utf8';

SELECT
  GROUP_CONCAT(DISTINCT
    CONCAT( tbsensor.descr )
  ) INTO @sql
FROM tbhistval, tbsensor
WHERE tbhistval.tbsensor_id = tbsensor.id
AND tbsensor.id = pval;

return @sql;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function getActuatorStatus
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `getActuatorStatus`(pid INT, pval INT) RETURNS int(4)
    DETERMINISTIC
BEGIN

SELECT IF(valueon=pval,1,0) into @psts
FROM vwactuator
WHERE id = pid;

return @psts;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure procConv
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `procConv`(IN pval INT, IN psensor INT, IN pmode INT, OUT CalculatedValue FLOAT)
BEGIN

SELECT formula into @formula 
from tbsensortype t, tbsensor s, tbconvformula f
where t.id = s.tbsensortype_id
and t.tbConvFormula_id = f.id 
and s.id = psensor;

SET @callString := CONCAT('Select ',@formula,'(',pval,',',pmode,') into @retVal');
PREPARE stmt FROM @callString;
EXECUTE stmt;
SELECT @retVal into CalculatedValue;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure setRemoteTriggerDelete
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `setRemoteTriggerDelete`()
BEGIN

DECLARE pTrigger INT;
DECLARE pTriggerGroup INT;
DECLARE pTriggerItem INT;
DECLARE pNode   INT;
DECLARE pSensor_pin   INT;

DECLARE exit_loop BOOLEAN;      

DECLARE delTrigger CURSOR FOR 
SELECT  a.trigger_id,
        a.triggergroup_id,
        a.triggeritem_id,
        a.node_id,
        a.sensor_pin
FROM _tbremotetrigger a
LEFT JOIN _vwremotetriggers b ON a.trigger_id = b.trigger_id 
     AND a.triggergroup_id = b.triggergroup_id 
     AND a.triggeritem_id = b.triggeritem_id 
WHERE b.trigger_id IS NULL
  AND b.triggergroup_id IS NULL
  AND b.triggeritem_id IS NULL;

DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

OPEN delTrigger;
	delTrigger_loop: LOOP

    FETCH delTrigger INTO pTrigger,pTriggerGroup,pTriggerItem,pNode,pSensor_pin;
	IF exit_loop THEN
		COMMIT;
		CLOSE delTrigger;
		LEAVE delTrigger_loop;
	ELSE
	SELECT currentstatus into @pNodeSts FROM tbnode WHERE id = pNode;
	IF @pNodeSts = 1 THEN-- node active
		-- insert into output table
		INSERT INTO tbdataout (timekey,type,V0,V1) values (millis(),5,pNode,pSensor_pin);
    END IF;
    -- delete record 
	DELETE FROM _tbremotetrigger 
		  WHERE trigger_id=pTrigger
            AND triggergroup_id=pTriggerGroup
            AND triggeritem_id=pTriggerItem;
	END IF;
END LOOP delTrigger_loop;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure setRemoteTriggerInsert
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `setRemoteTriggerInsert`()
BEGIN

	DECLARE pTrigger INT;
	DECLARE pTriggerGroup INT;
	DECLARE pTriggerItem INT;
    DECLARE pTriggerType INT;
	DECLARE pNode   INT;
	DECLARE pSensor_id   INT;
	DECLARE pSensor_pin   INT;
	DECLARE pActuator_pin   INT;
	DECLARE pValuemin INT;
	DECLARE pValueMax INT;
	DECLARE pOutput INT;
	DECLARE pValueTrue INT;
	DECLARE pValueFalse INT;
	DECLARE pFadingTime INT;
    DECLARE pTempTime INT;
	DECLARE pCurrentStatus INT;
    DECLARE exit_loop BOOLEAN;   
    

    DECLARE newTrigger CURSOR FOR 

 select a.trigger_id,
        a.triggergroup_id,
        a.triggeritem_id,
        a.trigger_type,
        a.node_id,
		a.sensor_id,
        a.sensor_pin,
        a.actuator_pin,
        a.valuemin,
        a.valuemax,
        a.output,
        a.actionTrue,
        a.actionFalse,
        a.fadingtime,
        a.temptime,
        a.currentstatus
FROM _vwremotetriggers a 
LEFT JOIN _tbremotetrigger b ON a.trigger_id = b.trigger_id 
     AND a.triggergroup_id = b.triggergroup_id 
     AND a.triggeritem_id = b.triggeritem_id 
WHERE b.trigger_id IS NULL
     AND b.triggergroup_id IS NULL
     AND b.triggeritem_id IS NULL;

		DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

	OPEN newTrigger;
		newTrigger_loop: LOOP

         FETCH newTrigger INTO pTrigger,pTriggerGroup,pTriggerItem,pTriggerType,pNode,pSensor_id,pSensor_pin,pActuator_pin,pValueMin,pValueMax,pOutput,pValueTrue,pValueFalse,pFadingTime,pTempTime,pCurrentStatus;
		IF exit_loop THEN
			COMMIT;
			CLOSE newTrigger;
			LEAVE newTrigger_loop;
		else		
			SELECT currentstatus into @pNodeSts FROM tbnode WHERE id = pNode;
			IF @pNodeSts = 1 THEN-- active
				INSERT INTO _tbremotetrigger (trigger_id,triggergroup_id,triggeritem_id,trigger_type,node_id,sensor_id,sensor_pin,actuator_pin,valuemin,valuemax,output,actionTrue,actionFalse,fadingtime,temptime,currentstatus)
				VALUES (pTrigger,pTriggerGroup,pTriggerItem,pTriggerType,pNode,pSensor_id,pSensor_pin,pActuator_pin,pValueMin,pValueMax,pOutput,pValueTrue,pValueFalse,pFadingTime,pTempTime,pCurrentStatus);
				-- IF NODE is active send remote triggers	

				CALL procConv(pValueMin,pSensor_id,1,@ret); 
				SET pValueMin = @ret;
				CALL procConv(pValueMax,pSensor_id,1,@ret); 
				SET pValueMax = @ret;

				INSERT INTO tbdataout (timekey
									  ,type
                                      ,V0
                                      ,V1
                                      ,V2
                                      ,V3
                                      ,V4
                                      ,V5
                                      ,V6
                                      ,V7
                                      ,V8
                                      ,V9
                                      ,V10) 
                values (millis()
                       ,4
					   ,pNode
                       ,pSensor_pin
                       ,pTriggerType
                       ,pActuator_pin
                       ,pValueMin
                       ,pValueMax
                       ,pOutput
                       ,pValueTrue
                       ,pValueFalse
                       ,pTempTime
                       ,pFadingTime);

			END IF;
		END IF;		

	END LOOP newTrigger_loop;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure setRemoteTriggerUpdate
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `setRemoteTriggerUpdate`()
BEGIN

	DECLARE pTriggerA INT;
	DECLARE pTriggerGroupA INT;
	DECLARE pTriggerItemA INT;
    DECLARE pTriggerTypeA INT;
	DECLARE pNodeA   INT;
	DECLARE pSensor_idA   INT;
	DECLARE pSensor_pinA   INT;
	DECLARE pActuator_pinA   INT;
	DECLARE pValueMinA INT;
	DECLARE pValueMaxA INT;
	DECLARE pCurrentStatusA INT;
	DECLARE pOutputA INT;
	DECLARE pValueTrueA INT;
	DECLARE pValueFalseA INT;
	DECLARE pFadingTimeA INT;
    DECLARE pTempTimeA INT;

    DECLARE exit_loop BOOLEAN;     
    DECLARE newTrigger CURSOR FOR 

select  distinct a.trigger_id,
        a.triggergroup_id,
        a.triggeritem_id,
        a.trigger_type,
        a.node_id,
		a.sensor_id,
        a.sensor_pin,
        a.actuator_pin,
        a.valuemin,
        a.valuemax,
        a.output,
        a.actionTrue,
        a.actionFalse,
        a.fadingtime,
        a.temptime,
        a.currentstatus
FROM _vwremotetriggers a 
LEFT JOIN _tbremotetrigger b USING
(trigger_id
,triggergroup_id
,triggeritem_id
,trigger_type
,node_id
,sensor_id
,sensor_pin
,actuator_pin
,valuemin
,valuemax
,output
,actionTrue
,actionFalse
,fadingtime
,temptime)
WHERE b.trigger_id IS NULL
AND b.triggergroup_id IS NULL
AND b.triggeritem_id IS NULL
AND b.trigger_type IS NULL
AND b.node_id IS NULL
AND b.sensor_pin IS NULL
AND b.actuator_pin IS NULL
AND b.valuemin IS NULL
AND b.valuemax IS NULL
AND b.output IS NULL
AND b.actionTrue IS NULL
AND b.actionFalse IS NULL
AND b.fadingtime IS NULL
AND b.temptime IS NULL
AND b.sensor_id IS NULL
AND b.currentstatus IS NULL;

      
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

	OPEN newTrigger;
		newTrigger_loop: LOOP

         FETCH newTrigger INTO pTriggerA,pTriggerGroupA,pTriggerItemA,pTriggerTypeA  
                              ,pNodeA,pSensor_idA,pSensor_pinA,pActuator_pinA
                              ,pValueMinA,pValueMaxA,pOutputA 
                              ,pValueTrueA,pValueFalseA,pFadingTimeA,pTempTimeA,pCurrentStatusA;
                              
		IF exit_loop THEN
			COMMIT;
			CLOSE newTrigger;
			LEAVE newTrigger_loop;
		else	
			SELECT currentstatus into @pNodeSts FROM tbnode WHERE id = pNodeA;
			IF @pNodeSts = 1 THEN-- node active
					UPDATE _tbremotetrigger SET triggeritem_id=pTriggerItemA,
					trigger_type=pTriggerTypeA, node_id=pNodeA, sensor_id=pSensor_idA, sensor_pin=pSensor_pinA, actuator_pin=pActuator_pinA,
					valuemin=pValueMinA, valuemax=pValueMaxA, output=pOutputA,
					actiontrue=pValueTrueA, actionfalse=pValueFalseA, fadingtime=pFadingTimeA, temptime=pTempTimeA, currentstatus=pCurrentStatusA
					WHERE trigger_id = pTriggerA
					AND triggergroup_id = pTriggerGroupA
					AND triggeritem_id = pTriggerItemA;

					CALL procConv(pValueMinA,pSensor_idA,1,@ret); 
					SET pValueMinA = @ret;
					CALL procConv(pValueMaxA,pSensor_idA,1,@ret); 
					SET pValueMaxA = @ret;

					INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10) 
					VALUES (millis(),4,pNodeA,pSensor_pinA,pTriggerTypeA,pActuator_pinA,pValueMinA,pValueMaxA,pOutputA,pValueTrueA,pValueFalseA,pTempTimeA,pFadingTimeA);

         END IF;
            
        END IF;
      END LOOP newTrigger_loop;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updAll
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updAll`()
BEGIN
call updEventStatus;
call updTimerStatus;
call updTriggerStatus;
call setLocalTriggerInsert;
call setLocalTriggerUpdate;
call setLocalTriggerDelete;
call setRemoteTriggerInsert;
call setRemoteTriggerUpdate;
call setRemoteTriggerDelete;
call updPopup;

CALL setProcTl(0,0);

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updEventStatus
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updEventStatus`()
BLOCK1: begin
DECLARE pid INT; 
DECLARE exit_loop BOOLEAN; 
DECLARE loopEvents CURSOR FOR 
select distinct id from tbevent;

DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;
	OPEN loopEvents; 
		loopEvents_loop: LOOP
        FETCH loopEvents INTO pid;
		IF exit_loop THEN
			COMMIT;
			CLOSE loopEvents;
			LEAVE loopEvents_loop;
		END IF;
		BLOCK2: begin
			DECLARE pValmin FLOAT;
			DECLARE pValmax FLOAT;
			DECLARE pCurrentval FLOAT;
			DECLARE pSequence INT;
			DECLARE pLogic INT;
			DECLARE pBracket VARCHAR(1);
			DECLARE exit_loop BOOLEAN;
	 
			DECLARE loopEventLogic CURSOR FOR 
			select valmin,valmax,currentvalue,sequence,logic,bracket 
			  from vwevents 
             where id = pid 
		  order by sequence,logic;

			DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;
			OPEN loopEventLogic; 
				SELECT FOUND_ROWS() into @recordcount;
				IF @recordcount > 0 THEN
					SET @pSQL = "select if(";
					loopEventLogic_loop: LOOP
					FETCH loopEventLogic INTO pValmin,pValmax,pCurrentval,pSequence,pLogic,pBracket;
						IF exit_loop THEN
							COMMIT;
							CLOSE loopEventLogic;
							LEAVE loopEventLogic_loop;
						ELSE	
							IF pCurrentval between pValmin and pValmax THEN
								SET @pSQL = concat(@pSQL,"1 ");
							ELSE
								SET @pSQL = concat(@pSQL,"0 ");
							END IF;
						
							IF pLogic = 0 THEN
								SET @pSQL = concat(@pSQL,"and ");
							ELSEIF pLogic = 1 THEN
								SET @pSQL = concat(@pSQL,"or ");
							END IF;

							IF pBracket is not null THEN
								SET @pSQL = concat(@pSQL,pBracket);
							END IF;

						END IF;
					END LOOP loopEventLogic_loop;
					SET @sts = 0;
					SET @pSQL = concat(@pSQL,",1,0) into @sts");
					-- insert into debug (debug) values ( concat(pid," " ,@pSQL));
					PREPARE stmt FROM @pSQL;
					EXECUTE stmt;
					DEALLOCATE PREPARE stmt;
					UPDATE tbevent set currentstatus = @sts where id = pid;
				ELSE
					UPDATE tbevent set currentstatus = 0 where id = pid;
				END IF;
		END BLOCK2;
	END LOOP loopEvents_loop;
END BLOCK1$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updTimerStatus
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updTimerStatus`()
BEGIN

DECLARE pid INT;
DECLARE pStatus BOOLEAN;
DECLARE exit_loop BOOLEAN;   

DECLARE loopTimers CURSOR FOR 
select t.id, if(d.D_status=1 and w.W_status=1 and m.M_status=1,1,0) as sts
from
tbtimer t
,
(select id,descr,
if(month(now())=1,m1,
if(month(now())=2,m2,
if(month(now())=3,m3,
if(month(now())=4,m4,
if(month(now())=5,m5,
if(month(now())=6,m6,
if(month(now())=7,m7,
if(month(now())=8,m8,
if(month(now())=9,m9,
if(month(now())=10,m10,
if(month(now())=11,m11,
if(month(now())=12,m12,0)))))))))))) as M_status
from tbtimermonthly) m
,
(select id,descr,if(weekday(now())=0,wd0,
if(weekday(now())=1,wd1,
if(weekday(now())=2,wd2,
if(weekday(now())=3,wd3,
if(weekday(now())=4,wd4,
if(weekday(now())=5,wd5,
if(weekday(now())=6,wd6,0))))))) as W_status
from tbtimerweekly) w
,
(select id, descr, 1 as D_status
from tbtimerdaily
where time(now()) between timestart and timeend
union
select id,descr, 0
from tbtimerdaily
where time(now()) not between timestart and timeend) d
where d.id = t.tbTimerDaily_id 
  and w.id = t.tbTimerWeekly_id
  and m.id = t.tbTimerMonthly_id;

DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

	OPEN loopTimers; 
		loopTimers_loop: LOOP
        FETCH loopTimers INTO pid,pStatus;
		IF exit_loop THEN
			COMMIT;
			CLOSE loopTimers;
			LEAVE loopTimers_loop;
		END IF;
			UPDATE tbtimer SET currentstatus = pStatus where id = pid;
	END LOOP loopTimers_loop;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updTriggerStatus
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updTriggerStatus`()
BEGIN

UPDATE tbtrigger a
LEFT JOIN vwtriggers b ON
    a.id = b.trigger_id
SET
    currentstatus = IF(b.event_status= 1 AND b.timer_status = 1,1,0);

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updateActuatorStatus
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateActuatorStatus`(IN psts INT, IN pid INT)
BEGIN

update tbactuator 
set currentistatus = psts
where id = pid;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updateHistDataTable
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateHistDataTable`(IN ptimer INT)
BEGIN
 DECLARE deadlock INT DEFAULT 0;
 DECLARE attempts INT DEFAULT 0;

 try_loop:WHILE (attempts<3) DO
   BEGIN
		DECLARE deadlock_detected CONDITION FOR 1213;
		DECLARE EXIT HANDLER FOR deadlock_detected
			BEGIN
				-- ROLLBACK;
				SET deadlock=-1;
			END;
		SET deadlock=0;
		START TRANSACTION;
			INSERT INTO tbhistval (tbsensor_id, value, date)
			SELECT s.id, s.currentvalue, now()
			FROM tbsensor s, tbsensortype t
			WHERE s.tbSensorType_id = t.id
			AND t.timer = ptimer
			AND s.tbStatus_id = 1;
		COMMIT;
	END;
	IF deadlock=0 THEN
		LEAVE try_loop;
	ELSE
		SET attempts = attempts+1;
		SELECT SLEEP(1);
	END IF;
 END WHILE try_loop;
	
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updateINData
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateINData`()
BEGIN

CALL updateINDataNode();
CALL updateINDataSensor();
CALL updateINDataActuator();

CALL setProcTl(0,1);
CALL setProcTl(1,0);

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updateINDataActuator
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateINDataActuator`()
BEGIN

    DECLARE tk   BIGINT(20);
	DECLARE ty   VARCHAR(1);
	DECLARE p0   INT; 
	DECLARE p1   INT; 
	DECLARE p2   INT;
	DECLARE p3   INT;
	DECLARE p4   INT;
	DECLARE p5   INT;
	DECLARE psts INT;
	DECLARE pConvVal FLOAT;

    DECLARE exit_loop BOOLEAN;   
    
    DECLARE dati CURSOR FOR 
		SELECT timekey,type,V0,V1,V2,V3,V4,V5 FROM tbdatain WHERE type = 2;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

	OPEN dati;
 
	dati_loop: LOOP
      FETCH dati INTO tk,ty,p0,p1,p2,p3,p4,p5;

			SELECT currentstatus into @nSts from tbnode WHERE id = p0;
			SELECT id into @aID FROM tbactuator WHERE tbnode_id = p0 AND pinnumber = p1;
			IF @nSts = 1 THEN
				UPDATE tbactuator 
					SET tbactuator.currentstatus = getActuatorStatus(id,p2)
						, tbactuator.currentvalue = p2
				WHERE tbactuator.id = @aID;
			ELSE
				UPDATE tbactuator
				SET   currentstatus=0
				WHERE tbactuator.id = @aID;
            END IF;
                
			DELETE from tbdatain 
			 WHERE timekey = tk
			   AND type = ty
		       AND V0 = p0
			   AND V1 = p1;	

		IF exit_loop THEN
			CLOSE dati;
			LEAVE dati_loop;
		END IF;

   END LOOP dati_loop;
 
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updateINDataNode
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateINDataNode`()
BEGIN

	DECLARE tk   BIGINT(20);
	DECLARE ty   INT;
	DECLARE p0   INT; 
	DECLARE p1   INT; 
	DECLARE p2   INT;
	DECLARE p3   INT;
	DECLARE p4   INT;

    DECLARE exit_loop BOOLEAN;   
    
    DECLARE dati CURSOR FOR 
		SELECT timekey,type,V0,V1,V2,V3,V4 FROM tbdatain WHERE type = 0;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

	OPEN dati;
 
	dati_loop: LOOP
      FETCH dati INTO tk,ty,p0,p1,p2,p3,p4;
			IF exit_loop THEN
				CLOSE dati;
				LEAVE dati_loop;
			ELSE
			UPDATE tbnode 
			   set tbnode.currentstatus = p4
                 -- , tbnode.signal = p3
				 , tbnode.lastupdate = now()
			where tbnode.id = p0;
			
			IF p4 = 0 THEN -- if node inactive set all actuator inactive
				UPDATE tbactuator
					set tbactuator.currentstatus = p4
                      , tbactuator.currentvalue = 0
				where tbactuator.tbnode_id = p0;				
			END IF;
                
			DELETE from tbdatain 
			WHERE timekey = tk;		
            
		END IF;
	
   END LOOP dati_loop;
 
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updateINDataSensor
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateINDataSensor`()
BEGIN

	DECLARE tk   BIGINT(20);
	DECLARE ty   INT;
	DECLARE p0   INT; 
	DECLARE p1   INT; 
	DECLARE p2   INT; 
	DECLARE p3   INT;
	DECLARE p4   INT;
	DECLARE p5   INT;
	DECLARE psts INT;
	DECLARE pConvVal FLOAT;

    DECLARE exit_loop BOOLEAN;   
    
    DECLARE dati CURSOR FOR 
		SELECT timekey,TYPE,V0,V1,V2,V3,V4 FROM tbdatain where tbdatain.type = 1;

	DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

	OPEN dati;
 
	dati_loop: LOOP
      FETCH dati INTO tk,ty,p0,p1,p2,p3,p4;
		IF exit_loop THEN
			CLOSE dati;
			LEAVE dati_loop;
		END IF;

			SELECT currentstatus into @nSts from tbnode WHERE id = p0;
            SELECT id into @sID FROM tbsensor WHERE tbsensor.tbnode_id = p0 AND tbsensor.pin_number = p1;
			IF @nSts = 1 THEN
 				CALL procConv(p2,@sID,0,pConvVal);
				UPDATE tbsensor 
				SET   tbsensor.currentvalue = pConvVal
					, tbsensor.currentstatus=1
					, tbsensor.lastupdate = NOW()
				WHERE id = @sID;
			ELSE
				UPDATE tbsensor 
				SET   tbsensor.currentstatus=0
				WHERE id = @sID;
            END IF;
      
			DELETE from tbdatain 
			 WHERE timekey = tk
			   AND type = ty
			   AND V0 = p0
			   AND V1 = p1;	
         
         SET exit_loop = FALSE;

   END LOOP dati_loop;

   call updEventStatus;
   call updTimerStatus;
   call updTriggerStatus;
   -- call processTriggerList;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- function millis
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `millis`() RETURNS bigint(20)
BEGIN

select conv(concat(substring(uid,16,3),substring(uid,10,4),substring(uid,1,8)),16,10) div 10000 - (141427 * 24 * 60 * 60 * 1000) into @millis
from (select uuid() uid) as alias;

RETURN @millis;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure setLocalTriggerDelete
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `setLocalTriggerDelete`()
BEGIN

DECLARE pTrigger INT;
DECLARE pTriggerGroup INT;
DECLARE pTriggerItem INT;
DECLARE pNode INT;
DECLARE pActuator_pin INT;
DECLARE pOutput INT;
DECLARE pAction INT;
DECLARE pTempTime INT;
DECLARE pFadingTime INT;

DECLARE exit_loop BOOLEAN;   

DECLARE newTrigger CURSOR FOR
SELECT y.trigger_id
      ,y.triggergroup_id
      ,y.triggeritem_id
	  ,y.node_id
	  ,y.actuator_pin
	  ,y.action
	  ,y.output
	  ,y.temptime
	  ,y.fadingtime
FROM _tbtemptrigger as y
JOIN 
(SELECT a.trigger_id
	   ,a.triggergroup_id
       ,a.triggeritem_id
FROM _tbtemptrigger a
LEFT JOIN _vwtriggers b ON a.trigger_id = b.trigger_id 
      AND a.triggergroup_id = b.triggergroup_id 
      AND a.triggeritem_id = b.triggeritem_id 
WHERE b.trigger_id IS NULL
  AND b.triggergroup_id IS NULL
  AND b.triggeritem_id IS NULL) x
ON  x.trigger_id = y.trigger_id 
AND x.triggergroup_id = y.triggergroup_id 
AND x.triggeritem_id = y.triggeritem_id;

DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

	OPEN newTrigger;
		newTrigger_loop: LOOP

		FETCH newTrigger INTO pTrigger,pTriggerGroup,pTriggerItem,pNode,pActuator_pin,pOutput,pAction,pTempTime,pFadingTime;
		IF exit_loop THEN
			COMMIT;
			CLOSE newTrigger;
			LEAVE newTrigger_loop;
		else
        
			DELETE FROM _tbtemptrigger 
			WHERE trigger_id = pTrigger AND triggergroup_id = pTriggerGroup AND triggeritem_id = pTriggerItem;
        
			INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5) VALUES (millis(),1,pNode,pActuator_pin,0,pOutput,0,0);
			
		END IF;		

	END LOOP newTrigger_loop;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure setLocalTriggerInsert
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `setLocalTriggerInsert`()
BEGIN

DECLARE pTrigger INT;
DECLARE pTriggerGroup INT;
DECLARE pTriggerItem INT;
DECLARE pNode INT;
DECLARE pActuator_pin INT;
DECLARE pOutput INT;
DECLARE pAction INT;
DECLARE pTempTime INT;
DECLARE pFadingTime INT;

DECLARE exit_loop BOOLEAN;   
DECLARE newTrigger CURSOR FOR

-- insert new triggers

	SELECT a.trigger_id
	      ,a.triggergroup_id
          ,a.triggeritem_id
		  ,a.node_id
		  ,a.actuator_pin
		  ,a.action
		  ,a.output
		  ,a.temptime
		  ,a.fadingtime
FROM _vwtriggers a 
LEFT JOIN _tbtemptrigger b ON a.trigger_id = b.trigger_id 
     AND a.triggergroup_id = b.triggergroup_id 
     AND a.triggeritem_id = b.triggeritem_id 
WHERE b.trigger_id IS NULL
     AND b.triggergroup_id IS NULL
     AND b.triggeritem_id IS NULL
     AND a.trigger_type = 0; -- local triggers only
     

DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

	OPEN newTrigger;
		newTrigger_loop: LOOP

		FETCH newTrigger INTO pTrigger,pTriggerGroup,pTriggerItem,pNode,pActuator_pin,pOutput,pAction,pTempTime,pFadingTime;
		IF exit_loop THEN
			COMMIT;
			CLOSE newTrigger;
			LEAVE newTrigger_loop;
		else
        
         INSERT INTO _tbtemptrigger (
		   trigger_id
	      ,triggergroup_id
          ,triggeritem_id
		  ,node_id
		  ,actuator_pin
		  ,action
		  ,output
		  ,temptime
		  ,fadingtime
          ,status)
		 VALUES (pTrigger,pTriggerGroup,pTriggerItem,pNode,pActuator_pin,pAction,pOutput,pTempTime,pFadingTime,0);
        
         INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5) VALUES (millis(),0,pNode,pActuator_pin,pAction,pOutput,pTempTime,pFadingTime);
			
		END IF;		

	END LOOP newTrigger_loop;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure setLocalTriggerUpdate
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `setLocalTriggerUpdate`()
BEGIN
DECLARE pTrigger INT;
DECLARE pTriggerGroup INT;
DECLARE pTriggerItem INT;
DECLARE pNode INT;
DECLARE pActuator_pin INT;
DECLARE pOutput INT;
DECLARE pAction INT;
DECLARE pTempTime INT;
DECLARE pFadingTime INT;

DECLARE exit_loop BOOLEAN;   
DECLARE newTrigger CURSOR FOR

-- insert new triggers

select y.trigger_id
	  ,y.triggergroup_id
      ,y.triggeritem_id
	  ,y.node_id
	  ,y.actuator_pin
      ,y.output
	  ,y.action
	  ,y.temptime
	  ,y.fadingtime
from 
  _vwtriggers as y 
  inner join (
SELECT a.trigger_id,a.triggergroup_id, a.triggeritem_id,a.action
FROM _tbtemptrigger a 
LEFT JOIN _vwtriggers b ON a.trigger_id = b.trigger_id 
     AND a.triggergroup_id = b.triggergroup_id 
     AND a.triggeritem_id = b.triggeritem_id 
WHERE a.action != b.action) as x 
     ON x.trigger_id = y.trigger_id
	 AND x.triggergroup_id = y.triggergroup_id
	 AND x.triggeritem_id = y.triggeritem_id
     AND y.trigger_type = 0;
     

DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

	OPEN newTrigger;
		newTrigger_loop: LOOP

		FETCH newTrigger INTO pTrigger,pTriggerGroup,pTriggerItem,pNode,pActuator_pin,pOutput,pAction,pTempTime,pFadingTime;
		IF exit_loop THEN
			COMMIT;
			CLOSE newTrigger;
			LEAVE newTrigger_loop;
		else
        
		UPDATE _tbtemptrigger SET action = pAction 
         WHERE trigger_id = pTrigger
	       AND triggergroup_id = pTriggerGroup
           AND triggeritem_id = pTriggerItem;
	
         INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5) VALUES (millis(),0,pNode,pActuator_pin,pAction,pOutput,pTempTime,pFadingTime);
			
		END IF;		

	END LOOP newTrigger_loop;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updActuatorStatus
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updActuatorStatus`(IN pid INT, IN psts INT)
BEGIN

update tbactuator 
set currentstatus = psts
where id = pid;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure setActuator
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `setActuator`(IN pActuator INT, IN pAction INT)
BEGIN

SELECT IF(pAction=1,valueon,valueoff) into @sts
  FROM vwactuator
WHERE id = pActuator;

INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5) 
SELECT millis()
     ,0
	 ,tbNode_id
	 ,pinnumber
	 , IF(pAction=1,valueon,valueoff)
	 , tbOutput_id
	 ,temptime
	 ,fadingtime
FROM vwactuator
WHERE id = pActuator;

call updActuatorStatus(pActuator, pAction);


END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure init
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `init`()
BEGIN
#truncate alarm tables
truncate table _tbremotetrigger;
truncate table _tbtemptrigger;
truncate table tbdataout;
truncate table tbdatain;
truncate table tbsendemail;
update tbactuator set currentstatus = 0;
update tbsensor set currentvalue = 0 where currentvalue IS NULL;
update tbtrigger set currentstatus = 0;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure updPopup
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `updPopup`()
BEGIN
BLOCK1: BEGIN

DECLARE pid INT;
DECLARE pSensorid INT;
DECLARE pTriggerid INT;
DECLARE pTriggerName VARCHAR(100);
DECLARE pAction VARCHAR(100);
DECLARE pActuator VARCHAR(100);

DECLARE exit_loop BOOLEAN;
DECLARE loopEvents CURSOR FOR

SELECT trigger_id,event_id,sensor,timer_name,action,actuator_name FROM vwtriggers;

DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

CREATE TEMPORARY TABLE IF NOT EXISTS sCount (id SMALLINT);

 OPEN loopEvents;

  loopEvents_loop: LOOP

        FETCH loopEvents INTO pTriggerid,pid,pSensorid,pTriggerName,pAction,pActuator;

  IF exit_loop THEN

   COMMIT;
   CLOSE loopEvents;
   LEAVE loopEvents_loop;

  END IF;

  BLOCK2: BEGIN

   DECLARE pValmin FLOAT;
   DECLARE pValmax FLOAT;
   DECLARE pCurrentval FLOAT;
   DECLARE pSequence INT;
   DECLARE pLogic INT;
   DECLARE pBracket VARCHAR(1);
   DECLARE pSensor VARCHAR(100);
   DECLARE pSensorid INT;
   DECLARE pXid INT;
   DECLARE exit_loop BOOLEAN;

   DECLARE loopEventLogic CURSOR FOR
   SELECT valmin,valmax,currentvalue,sequence,logic,bracket,sensor,tbSensor_id FROM vwevents WHERE id = pid ORDER BY sequence,logic;

   DECLARE loopCount CURSOR FOR
   SELECT id FROM sCount;
    
   DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;
   
   SELECT word(100) into @p100;
   SELECT word(101) into @p101;
   SELECT word(102) into @p102;
   SELECT word(103) into @p103;
   SELECT word(104) into @p104; 
   
   SET @pSQL = concat(@p100," ");
   OPEN loopEventLogic;
    loopEventLogic_loop: LOOP
    FETCH loopEventLogic INTO pValmin,pValmax,pCurrentval,pSequence,pLogic,pBracket,pSensor,pSensorid;
     IF exit_loop THEN
     COMMIT;
     CLOSE loopEventLogic;
     LEAVE loopEventLogic_loop;
     ELSE
		SET @pSQL = concat(@pSQL,pSensor);
        SET @pSQL = concat(@pSQL," ");
		SET @pSQL = concat(@pSQL,@p101);
        SET @pSQL = concat(@pSQL," ");
		SET @pSQL = concat(@pSQL,pValmin);
        SET @pSQL = concat(@pSQL," ");
		SET @pSQL = concat(@pSQL,@p102);
        SET @pSQL = concat(@pSQL," ");
		SET @pSQL = concat(@pSQL,pValmax);
        SET @pSQL = concat(@pSQL," ");

      IF pLogic = 0 THEN
       SET @pSQL = concat(@pSQL,@p102);
       SET @pSQL = concat(@pSQL," ");
      ELSEIF pLogic = 1 THEN
       SET @pSQL = concat(@pSQL,@p103);
       SET @pSQL = concat(@pSQL," ");
      END IF;

      IF pBracket IS NOT NULL THEN
       SET @pSQL = concat(@pSQL,pBracket);
      END IF;
      
      INSERT INTO sCount (id) VALUES (pSensorid);
	  COMMIT;
     END IF;
    END LOOP loopEventLogic_loop;
 
   	OPEN loopCount;
    BEGIN
		DECLARE exit_count BOOLEAN DEFAULT FALSE;
		DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_count = TRUE;
		SET @cwhat = concat(" ", pAction, " ", pActuator);
		loopCount_loop: LOOP
		FETCH loopCount INTO pXid;
		IF exit_count THEN
		COMMIT;
		CLOSE loopCount;
		LEAVE loopCount_loop;
		ELSE
            SET @pTriggerName = concat("| ",pTriggerName," |");
			REPLACE INTO tbpopup (tbTrigger_id,tbSensor_id,cwhy,cwhen,cwhat) VALUES (pTriggerid,pXid,@pSQL,@pTriggerName,@cwhat);
			COMMIT;
		END IF;
		END LOOP loopCount_loop;
		DELETE FROM sCount;
    END;
  END BLOCK2;
 END LOOP loopEvents_loop;
 DROP TABLE sCount;
END BLOCK1;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function word
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$


CREATE DEFINER=`root`@`localhost` FUNCTION `word`(pid INT) RETURNS varchar(100) CHARSET utf8
BEGIN

SELECT wording into @wrd FROM vwtranslation WHERE id = pid;

RETURN @wrd;
END

$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure setProcTl
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `setProcTl`(IN pval INT, IN pId INT)
BEGIN
START TRANSACTION;
UPDATE tbproctl SET sts = pval WHERE id = pId;
COMMIT;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function getstatusicon
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` FUNCTION `getstatusicon`(psts INT) RETURNS varchar(10) CHARSET utf8 COLLATE utf8_bin
BEGIN

SELECT IF(psts=1,'active','inactive') into @psts;

return @psts;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure setFunction
-- -----------------------------------------------------

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `setFunction`(IN pFunction INT, IN pMode INT)
BEGIN

DECLARE pTrigger INT;
DECLARE pTriggerGroup INT;
DECLARE pTriggerItem INT;
DECLARE pNode INT;
DECLARE pActuator_pin INT;
DECLARE pOutput INT;
DECLARE pValueTrue INT;
DECLARE pValueFalse INT;
DECLARE pTempTime INT;
DECLARE pFadingTime INT;
DECLARE exit_loop BOOLEAN;   

DECLARE newActuators CURSOR FOR

-- set actuators
	SELECT a.trigger_id
		  ,a.node_id
		  ,a.actuator_pin
          ,a.output
          ,a.valuetrue
		  ,a.valuefalse 
		  ,a.temptime
		  ,a.fadingtime
FROM vwtriggers a 
JOIN tbfunctiontrigger ft
WHERE a.trigger_id = ft.tbtrigger_id
  AND ft.tbfunction_id = pFunction;
     

DECLARE CONTINUE HANDLER FOR NOT FOUND SET exit_loop = TRUE;

SET @p1 = pFunction;
SET @p2 = pMode;

	OPEN newActuators;
		newActuators_loop: LOOP

		FETCH newActuators INTO pTrigger,pNode,pActuator_pin,pOutput,pValueTrue,pValueFalse,pTempTime,pFadingTime;
		IF exit_loop THEN
			COMMIT;
			CLOSE newActuators;
			LEAVE newActuators_loop;
		ELSE
       
			-- reset actuators
			INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5) VALUES (millis(),0,pNode,pActuator_pin,pValueFalse,pOutput,pTempTime,pFadingTime);
        
			-- set triggers
			UPDATE tbtrigger SET tbStatus_id = pMode WHERE id = pTrigger;
			
		END IF;		

	END LOOP newActuators_loop;
    
    UPDATE tbfunction SET currentstatus = @p2 WHERE id = @p1;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- View `dh`.`_vwremotetriggers`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_vwremotetriggers`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_vwremotetriggers` AS
    select 
        `t`.`id` AS `trigger_id`,
        `g`.`id` AS `triggergroup_id`,
        `i`.`id` AS `triggeritem_id`,
        `t`.`tbTriggerType_id` AS `trigger_type`,
        `ss`.`tbNode_id` AS `node_id`,
        `ss`.`id` AS `sensor_id`,
        `ss`.`pin_number` AS `sensor_pin`,
        `a`.`pinnumber` AS `actuator_pin`,
        `el`.`valmin` AS `valuemin`,
        `el`.`valmax` AS `valuemax`,
        `b`.`tbOutput_id` AS `output`,
        `ct`.`value` AS `actionTrue`,
        `cf`.`value` AS `actionFalse`,
        `a`.`fadingtime` AS `fadingtime`,
        `a`.`temptime` AS `temptime`,
        `t`.`currentstatus` AS `currentstatus`
    from
        (((((((((((((`tbevent` `e`
        join `tbtrigger` `t`)
        join `tbactiongroup` `g`)
        join `tbactionitem` `i`)
        join `tbaction` `ct`)
        join `tbaction` `cf`)
        join `tbtriggertype` `y`)
        join `tbtimer` `x`)
        join `tbactuator` `a`)
        join `tbactuatortype` `b`)
        join `vwtranslation` `tr`)
        join `tbsensor` `ss`)
        join `tbeventlogic` `el`)
        join `tbnode` `n`)
    where
        ((`t`.`tbEvent_id` = `e`.`id`)
            and (`t`.`tbActionGroup_id` = `g`.`id`)
            and (`t`.`tbTimer_id` = `x`.`id`)
            and (`t`.`tbTriggerType_id` = `y`.`id`)
            and (`g`.`id` = `i`.`tbActionGroup_id`)
            and (`i`.`tbActuators_id` = `a`.`id`)
            and (`i`.`actiontrue` = `ct`.`id`)
            and (`i`.`actionfalse` = `cf`.`id`)
            and (`a`.`tbActuatorType_id` = `b`.`id`)
            and (`b`.`tbTranslation_id` = `tr`.`id`)
            and (`el`.`tbSensor_id` = `ss`.`id`)
            and (`e`.`id` = `el`.`tbEvent_id`)
            and (`t`.`tbTriggerType_id` <> 0)
            and (`ss`.`tbNode_id` = `n`.`id`)
            and (`t`.`tbStatus_id` = 1)
            and (`ss`.`tbStatus_id` = 1)
            and (`n`.`currentstatus` = 1)
            and (`n`.`tbStatus_id` = 1))
    order by `t`.`id` , `g`.`id` , `i`.`id` , `i`.`sequence`;

-- -----------------------------------------------------
-- View `dh`.`_vwtriggers`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_vwtriggers`;
USE `dh`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `dh`.`_vwtriggers` AS select `vwtriggers`.`trigger_id` AS `trigger_id`,`vwtriggers`.`triggergroup_id` AS `triggergroup_id`,`vwtriggers`.`triggeritem_id` AS `triggeritem_id`,`vwtriggers`.`trigger_type` AS `trigger_type`,`vwtriggers`.`node_id` AS `node_id`,`vwtriggers`.`actuator_pin` AS `actuator_pin`,if((`vwtriggers`.`trigger_currentstatus` = 0),`vwtriggers`.`valueFalse`,`vwtriggers`.`valueTrue`) AS `action`,`vwtriggers`.`trigger_status` AS `trigger_status`,`vwtriggers`.`trigger_currentstatus` AS `trigger_currentstatus`,`vwtriggers`.`output` AS `output`,`vwtriggers`.`fadingtime` AS `fadingtime`,`vwtriggers`.`temptime` AS `temptime` from `dh`.`vwtriggers` where ((`vwtriggers`.`trigger_status` = 1) and (`vwtriggers`.`trigger_type` = 0));

-- -----------------------------------------------------
-- View `dh`.`vwevents`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwevents`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `dh`.`vwevents` AS
    SELECT 
        `e`.`id` AS `id`,
        `e`.`descr` AS `descr`,
        GETSTATUSICON(`e`.`currentstatus`) AS `event_status`,
        `n`.`descr` AS `node`,
        `l`.`tbSensor_id` AS `tbSensor_id`,
        `s`.`descr` AS `sensor`,
        `i`.`icon` AS `sensor_type`,
        `l`.`valmin` AS `valmin`,
        `l`.`valmax` AS `valmax`,
        `s`.`currentvalue` AS `currentvalue`,
        GETSTATUSICON(`s`.`currentstatus`) AS `sensorcurrentstatus`,
        `l`.`sequence` AS `sequence`,
        `l`.`logic` AS `logic`,
        IF((`l`.`logic` IS NOT NULL),
            IF((`l`.`logic` = 0),
                'AND',
                IF((`l`.`logic` = 1), 'OR', NULL)),
            NULL) AS `s_logic`,
        `l`.`bracket` AS `bracket`,
        `st`.`scalemin` AS `scalemin`,
        `st`.`scalemax` AS `scalemax`,
        `l`.`id` AS `eventlogic_id`
    FROM
        (((((`dh`.`tbevent` `e`
        JOIN `dh`.`tbnode` `n`)
        JOIN `dh`.`tbsensortype` `st`)
        JOIN `dh`.`tbicon` `i`)
        LEFT JOIN `dh`.`tbeventlogic` `l` ON ((`e`.`id` = `l`.`tbEvent_id`)))
        LEFT JOIN `dh`.`tbsensor` `s` ON ((`s`.`id` = `l`.`tbSensor_id`)))
    WHERE
        ((`s`.`tbNode_id` = `n`.`id`)
            AND (`s`.`tbSensorType_id` = `st`.`id`)
            AND (`st`.`tbIcon_id` = `i`.`id`))
    ORDER BY `l`.`tbEvent_id` , `l`.`sequence`;

-- -----------------------------------------------------
-- View `dh`.`vwnodes`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwnodes`;
USE `dh`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `dh`.`vwnodes` AS select `dh`.`tbnode`.`id` AS `id`,`dh`.`tbnode`.`tbStatus_id` AS `status`,`dh`.`tbnode`.`xbee_low_address` AS `xbee_low_address`,`dh`.`tbnode`.`xbee_high_address` AS `xbee_high_address`,`dh`.`tbnode`.`tbNodeType_id` AS `nodeType` from `dh`.`tbnode` where (`dh`.`tbnode`.`tbStatus_id` = 1);

-- -----------------------------------------------------
-- View `dh`.`vwsensors`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwsensors`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwsensors` AS
    select 
        `s`.`id` AS `Sensor_id`,
        `n`.`id` AS `NodeId`,
        `n`.`tbNodeType_id` AS `tbNodeType_id`,
        `s`.`tbSensorType_id` AS `tbSensorType_id`,
        `s`.`tbStatus_id` AS `tbStatus_id`,
        `s`.`pin_number` AS `pin_number`,
        `p`.`pindex` AS `Timer`,
        `s`.`currentvalue` AS `currentvalue`
    from
        ((((`tbnode` `n`
        join `tbsensor` `s`)
        join `tbsensortype` `t`)
        join `tbparam` `p`)
        join `tbnodetype` `d`)
    where
        ((`n`.`id` = `s`.`tbNode_id`)
            and (`n`.`tbNodeType_id` = `d`.`id`)
            and (`t`.`timer` = `p`.`id`)
            and (`s`.`tbSensorType_id` = `t`.`id`));

-- -----------------------------------------------------
-- View `dh`.`vwtranslation`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtranslation`;
USE `dh`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `dh`.`vwtranslation` AS select `a`.`id` AS `id`,`b`.`wording` AS `wording` from ((`dh`.`tbtranslation` `a` join `dh`.`tbtranslationitem` `b`) join `dh`.`users` `c`) where ((`a`.`id` = `b`.`tbTranslation_id`) and (`b`.`tbLanguage_id` = `c`.`user_language`));

-- -----------------------------------------------------
-- View `dh`.`vwtriggers`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtriggers`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwtriggers` AS
    SELECT 
        `t`.`id` AS `trigger_id`,
        `g`.`id` AS `triggergroup_id`,
        `i`.`id` AS `triggeritem_id`,
        `t`.`tbStatus_id` AS `trigger_status`,
        `t`.`currentstatus` AS `trigger_currentstatus`,
        `t`.`tbTriggerType_id` AS `trigger_type`,
        `e`.`id` AS `event_id`,
        `e`.`currentstatus` AS `event_status`,
        `y`.`id` AS `triggerType_id`,
        `g`.`id` AS `actiongroup_id`,
        `i`.`sequence` AS `sequence`,
        `ct`.`value` AS `valueTrue`,
        `cf`.`value` AS `valueFalse`,
        `a`.`id` AS `actuator_id`,
        `a`.`descr` AS `actuator_name`,
        `b`.`id` AS `actuatorType_id`,
        `tr`.`wording` AS `actuatorType`,
        `a`.`tbNode_id` AS `node_id`,
        `a`.`pinnumber` AS `actuator_pin`,
        `a`.`currentstatus` AS `actuator_status`,
        `a`.`fadingtime` AS `fadingtime`,
        `a`.`temptime` AS `temptime`,
        `b`.`tbOutput_id` AS `output`,
        `x`.`id` AS `timer_id`,
        `x`.`descr` AS `timer_name`,
        `x`.`currentstatus` AS `timer_status`,
        `ss`.`id` AS `sensor`,
        `g`.`descr` AS `action`,
        `r`.`descr` AS `room`
    FROM
        (((((((((((((((`tbevent` `e`
        JOIN `tbtrigger` `t`)
        JOIN `tbactiongroup` `g`)
        JOIN `tbactionitem` `i`)
        JOIN `tbaction` `ct`)
        JOIN `tbaction` `cf`)
        JOIN `tbtriggertype` `y`)
        JOIN `tbstatus` `s`)
        JOIN `tbtimer` `x`)
        JOIN `tbactuator` `a`)
        JOIN `tbactuatortype` `b`)
        JOIN `vwtranslation` `tr`)
        JOIN `tbeventlogic` `el`)
        JOIN `tbsensor` `ss`)
        JOIN `tbnode` `n`)
        JOIN `tbroom` `r`)
    WHERE
        ((`t`.`tbEvent_id` = `e`.`id`)
            AND (`t`.`tbStatus_id` = `s`.`id`)
            AND (`t`.`tbActionGroup_id` = `g`.`id`)
            AND (`t`.`tbTimer_id` = `x`.`id`)
            AND (`t`.`tbTriggerType_id` = `y`.`id`)
            AND (`g`.`id` = `i`.`tbActionGroup_id`)
            AND (`i`.`tbActuators_id` = `a`.`id`)
            AND (`i`.`actiontrue` = `ct`.`id`)
            AND (`i`.`actionfalse` = `cf`.`id`)
            AND (`a`.`tbActuatorType_id` = `b`.`id`)
            AND (`b`.`tbTranslation_id` = `tr`.`id`)
            AND (`e`.`id` = `el`.`tbEvent_id`)
            AND (`el`.`tbSensor_id` = `ss`.`id`)
            AND (`ss`.`tbNode_id` = `n`.`id`)
            AND (`n`.`tbRoom_id` = `r`.`id`));

-- -----------------------------------------------------
-- View `dh`.`vwactuator`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwactuator`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwactuator` AS
    select 
        `a`.`id` AS `id`,
        `a`.`tbActuatorType_id` AS `tbActuatorType_id`,
        `a`.`tbNode_id` AS `tbNode_id`,
        `a`.`descr` AS `descr`,
        `a`.`pinnumber` AS `pinnumber`,
        `a`.`currentstatus` AS `currentstatus`,
        `a`.`currentvalue` AS `currentvalue`,
        `b`.`tbOutput_id` AS `tbOutput_id`,
        `b`.`valueon` AS `valueon`,
        `b`.`valueoff` AS `valueoff`,
        `a`.`temptime` AS `temptime`,
        `a`.`fadingtime` AS `fadingtime`,
        `b`.`tbIcon_id` AS `tbIcon_id`,
        `i`.`icon` AS `actuatortypeicon`,
        `c`.`hexCode` AS `actuatortypecolor`
    from
        (((`tbactuator` `a`
        join `tbactuatortype` `b`)
        join `tbicon` `i`)
        join `tbcolor` `c`)
    where
        ((`a`.`tbActuatorType_id` = `b`.`id`)
            and (`b`.`tbIcon_id` = `i`.`id`)
            and (`b`.`tbColor_id` = `c`.`id`));

-- -----------------------------------------------------
-- View `dh`.`vwdashboardsensorshomepage`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwdashboardsensorshomepage`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `dh`.`vwdashboardsensorshomepage` AS
    SELECT 
        `s`.`id` AS `id`,
        `s`.`tbNode_id` AS `tbNode_id`,
        `s`.`descr` AS `descr`,
        `s`.`pin_number` AS `pin_number`,
        `s`.`currentvalue` AS `currentvalue`,
        `it`.`icon` AS `status`,
        `s`.`lastupdate` AS `lastupdate`,
        `t`.`scalemin` AS `scalemin`,
        `t`.`scalemax` AS `scalemax`,
        `c`.`hexCode` AS `hexCode`,
        `i`.`icon` AS `sensor_type`,
        `um`.`descr` AS `um`,
        `s`.`addtohomepage` AS `addtohomepage`,
        `ri`.`icon` AS `room`,
        `r`.`descr` AS `roomname`
    FROM
        (((((((((`dh`.`tbsensor` `s`
        JOIN `dh`.`tbsensortype` `t`)
        JOIN `dh`.`tbcolor` `c`)
        JOIN `dh`.`tbnode` `n`)
        JOIN `dh`.`tbroom` `r`)
        JOIN `dh`.`tbicon` `i`)
        JOIN `dh`.`tbicon` `it`)
        JOIN `dh`.`tbicon` `ri`)
        JOIN `dh`.`tbstatus` `st`)
        JOIN `dh`.`tbum` `um`)
    WHERE
        ((`s`.`tbSensorType_id` = `t`.`id`)
            AND (`t`.`tbColor_id` = `c`.`id`)
            AND (`t`.`tbIcon_id` = `i`.`id`)
            AND (`s`.`tbStatus_id` = `st`.`id`)
            AND (`st`.`tbIcon_id` = `it`.`id`)
            AND (`t`.`tbum_id` = `um`.`id`)
            AND (`s`.`tbNode_id` = `n`.`id`)
            AND (`n`.`tbRoom_id` = `r`.`id`)
            AND (`r`.`tbicon_id` = `ri`.`id`)
            AND (`s`.`tbStatus_id` = 1));

-- -----------------------------------------------------
-- View `dh`.`vwdashboardactuators`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwdashboardactuators`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwdashboardactuators` AS
    select 
        `a`.`id` AS `id`,
        `a`.`tbActuatorType_id` AS `tbActuatorType_id`,
        `a`.`tbNode_id` AS `tbNode_id`,
        `a`.`descr` AS `descr`,
        `a`.`pinnumber` AS `pinnumber`,
        `a`.`currentstatus` AS `currentstatus`,
        getstatusicon(`a`.`currentstatus`) AS `currentstatusicon`,
        `a`.`currentvalue` AS `currentvalue`,
        `v`.`icon` AS `status`,
        `a`.`tbOutput_id` AS `tbOutput_id`,
        `a`.`valueon` AS `valueon`,
        `a`.`valueoff` AS `valueoff`,
        `a`.`temptime` AS `temptime`,
        `a`.`fadingtime` AS `fadingtime`,
        `a`.`actuatortypeicon` AS `actuatortypeicon`,
        `a`.`actuatortypecolor` AS `actuatortypecolor`,
        if((`a`.`currentstatus` = 1),
            'active',
            'inactive') AS `currentstatuscolor`
    from
        (((`vwactuator` `a`
        join `tbicon` `c`)
        join `tbstatus` `s`)
        join `tbicon` `v`)
    where
        ((`a`.`tbIcon_id` = `c`.`id`)
            and (`a`.`currentstatus` = `s`.`id`)
            and (`s`.`tbIcon_id` = `v`.`id`));

-- -----------------------------------------------------
-- View `dh`.`vwcharts`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwcharts`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwcharts` AS
    SELECT 
        `c`.`id` AS `id`,
        `t`.`wording` AS `wording`,
        `i`.`icon` AS `icon`
    FROM
        ((`tbchart` `c`
        JOIN `vwtranslation` `t`)
        JOIN `tbicon` `i`)
    WHERE
        ((`c`.`tbTranslation_id` = `t`.`id`)
            AND (`c`.`tbIcon_id` = `i`.`id`));

-- -----------------------------------------------------
-- View `dh`.`vwnodesdashboard`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwnodesdashboard`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `dh`.`vwnodesdashboard` AS
    SELECT 
        `n`.`id` AS `id`,
        `n`.`tbStatus_id` AS `status`,
        `ii`.`icon` AS `tipo`,
        `n`.`descr` AS `descr`,
        GETSTATUSICON(`n`.`currentstatus`) AS `currentstatus`,
        `t`.`scalemin` AS `scalemin`,
        `t`.`scalemax` AS `scalemax`,
        `c`.`hexCode` AS `color`,
        `n`.`signalvalue` AS `signal`,
        `i`.`icon` AS `room`
    FROM
        (((((`dh`.`tbnode` `n`
        JOIN `dh`.`tbnodetype` `t`)
        JOIN `dh`.`tbroom` `ro`)
        JOIN `dh`.`tbcolor` `c`)
        JOIN `dh`.`tbicon` `i`)
        JOIN `dh`.`tbicon` `ii`)
    WHERE
        ((`n`.`tbNodeType_id` = `t`.`id`)
            AND (`t`.`tbColor_id` = `c`.`id`)
            AND (`t`.`tbIcon_id` = `ii`.`id`)
            AND (`ro`.`id` = `n`.`tbRoom_id`)
            AND (`ro`.`tbicon_id` = `i`.`id`)
            AND (`n`.`id` <> 1));

-- -----------------------------------------------------
-- View `dh`.`vwdashboardsensorsdetails`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwdashboardsensorsdetails`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `dh`.`vwdashboardsensorsdetails` AS
    SELECT 
        `s`.`id` AS `id`,
        `s`.`tbNode_id` AS `tbNode_id`,
        `s`.`descr` AS `descr`,
        `s`.`pin_number` AS `pin_number`,
        `s`.`currentvalue` AS `currentvalue`,
        `it`.`icon` AS `status`,
        `s`.`lastupdate` AS `lastupdate`,
        `t`.`scalemin` AS `scalemin`,
        `t`.`scalemax` AS `scalemax`,
        `c`.`hexCode` AS `hexCode`,
        `i`.`icon` AS `sensor_type`,
        `s`.`currentstatus` AS `currentstatus`,
        `um`.`descr` AS `um`
    FROM
        (((((((`dh`.`tbsensor` `s`
        JOIN `dh`.`tbsensortype` `t`)
        JOIN `dh`.`tbcolor` `c`)
        JOIN `dh`.`tbicon` `i`)
        JOIN `dh`.`tbicon` `it`)
        JOIN `dh`.`tbstatus` `st`)
        JOIN `dh`.`tbicon` `ii`)
        JOIN `dh`.`tbum` `um`)
    WHERE
        ((`s`.`tbSensorType_id` = `t`.`id`)
            AND (`t`.`tbColor_id` = `c`.`id`)
            AND (`t`.`tbIcon_id` = `i`.`id`)
            AND (`s`.`tbStatus_id` = `st`.`id`)
            AND (`st`.`tbIcon_id` = `it`.`id`)
            AND (`t`.`tbIcon_id` = `ii`.`id`)
            AND (`t`.`tbUm_id` = `um`.`id`)
            AND (`s`.`tbStatus_id` = 1));

-- -----------------------------------------------------
-- View `dh`.`vwtriggersdashboard`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtriggersdashboard`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwtriggersdashboard` AS
    SELECT 
        `t`.`id` AS `id`,
        `ss`.`id` AS `sensor_id`,
        `el`.`valmin` AS `valuemin`,
        `el`.`valmax` AS `valuemax`,
        `t`.`currentstatus` AS `currentstatus`,
        `a`.`fadingtime` AS `fadingtime`,
        `a`.`temptime` AS `temptime`,
        `t`.`tbStatus_id` AS `tbStatus_id`,
        `tt`.`icon` AS `trigger_type`,
        `ta`.`icon` AS `actuator_icon`,
        `ts`.`icon` AS `currentstatus_icon`,
        `a`.`tbNode_id` AS `node`,
        `a`.`id` AS `actuator_id`
    FROM
        (((((((((((((((((`tbevent` `e`
        JOIN `tbtrigger` `t`)
        JOIN `tbactiongroup` `g`)
        JOIN `tbactionitem` `i`)
        JOIN `tbaction` `ct`)
        JOIN `tbaction` `cf`)
        JOIN `tbtriggertype` `y`)
        JOIN `tbtimer` `x`)
        JOIN `tbactuator` `a`)
        JOIN `tbactuatortype` `b`)
        JOIN `vwtranslation` `tr`)
        JOIN `tbsensor` `ss`)
        JOIN `tbeventlogic` `el`)
        JOIN `tbnode` `n`)
        JOIN `tbicon` `tt`)
        JOIN `tbicon` `ta`)
        JOIN `tbstatus` `st`)
        JOIN `tbicon` `ts`)
    WHERE
        ((`t`.`tbEvent_id` = `e`.`id`)
            AND (`t`.`tbActionGroup_id` = `g`.`id`)
            AND (`t`.`tbTimer_id` = `x`.`id`)
            AND (`t`.`tbTriggerType_id` = `y`.`id`)
            AND (`g`.`id` = `i`.`tbActionGroup_id`)
            AND (`i`.`tbActuators_id` = `a`.`id`)
            AND (`i`.`actiontrue` = `ct`.`id`)
            AND (`i`.`actionfalse` = `cf`.`id`)
            AND (`a`.`tbActuatorType_id` = `b`.`id`)
            AND (`b`.`tbTranslation_id` = `tr`.`id`)
            AND (`el`.`tbSensor_id` = `ss`.`id`)
            AND (`e`.`id` = `el`.`tbEvent_id`)
            AND (`t`.`tbTriggerType_id` <> 0)
            AND (`ss`.`tbNode_id` = `n`.`id`)
            AND (`y`.`tbIcon_id` = `tt`.`id`)
            AND (`ss`.`tbStatus_id` = 1)
            AND (`n`.`tbStatus_id` = 1)
            AND (`b`.`tbIcon_id` = `ta`.`id`)
            AND (`t`.`currentstatus` = `st`.`id`)
            AND (`st`.`tbIcon_id` = `ts`.`id`))
    ORDER BY `t`.`id` , `g`.`id` , `i`.`id` , `i`.`sequence`;

-- -----------------------------------------------------
-- View `dh`.`vwtriggers`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtriggers`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwtriggers` AS
    SELECT 
        `t`.`id` AS `trigger_id`,
        `g`.`id` AS `triggergroup_id`,
        `i`.`id` AS `triggeritem_id`,
        `t`.`tbStatus_id` AS `trigger_status`,
        `t`.`currentstatus` AS `trigger_currentstatus`,
        `t`.`tbTriggerType_id` AS `trigger_type`,
        `e`.`id` AS `event_id`,
        `e`.`currentstatus` AS `event_status`,
        `y`.`id` AS `triggerType_id`,
        `g`.`id` AS `actiongroup_id`,
        `i`.`sequence` AS `sequence`,
        `ct`.`value` AS `valueTrue`,
        `cf`.`value` AS `valueFalse`,
        `a`.`id` AS `actuator_id`,
        `a`.`descr` AS `actuator_name`,
        `b`.`id` AS `actuatorType_id`,
        `tr`.`wording` AS `actuatorType`,
        `a`.`tbNode_id` AS `node_id`,
        `a`.`pinnumber` AS `actuator_pin`,
        `a`.`currentstatus` AS `actuator_status`,
        `a`.`fadingtime` AS `fadingtime`,
        `a`.`temptime` AS `temptime`,
        `b`.`tbOutput_id` AS `output`,
        `x`.`id` AS `timer_id`,
        `x`.`descr` AS `timer_name`,
        `x`.`currentstatus` AS `timer_status`,
        `ss`.`id` AS `sensor`,
        `g`.`descr` AS `action`,
        `r`.`descr` AS `room`
    FROM
        (((((((((((((((`tbevent` `e`
        JOIN `tbtrigger` `t`)
        JOIN `tbactiongroup` `g`)
        JOIN `tbactionitem` `i`)
        JOIN `tbaction` `ct`)
        JOIN `tbaction` `cf`)
        JOIN `tbtriggertype` `y`)
        JOIN `tbstatus` `s`)
        JOIN `tbtimer` `x`)
        JOIN `tbactuator` `a`)
        JOIN `tbactuatortype` `b`)
        JOIN `vwtranslation` `tr`)
        JOIN `tbeventlogic` `el`)
        JOIN `tbsensor` `ss`)
        JOIN `tbnode` `n`)
        JOIN `tbroom` `r`)
    WHERE
        ((`t`.`tbEvent_id` = `e`.`id`)
            AND (`t`.`tbStatus_id` = `s`.`id`)
            AND (`t`.`tbActionGroup_id` = `g`.`id`)
            AND (`t`.`tbTimer_id` = `x`.`id`)
            AND (`t`.`tbTriggerType_id` = `y`.`id`)
            AND (`g`.`id` = `i`.`tbActionGroup_id`)
            AND (`i`.`tbActuators_id` = `a`.`id`)
            AND (`i`.`actiontrue` = `ct`.`id`)
            AND (`i`.`actionfalse` = `cf`.`id`)
            AND (`a`.`tbActuatorType_id` = `b`.`id`)
            AND (`b`.`tbTranslation_id` = `tr`.`id`)
            AND (`e`.`id` = `el`.`tbEvent_id`)
            AND (`el`.`tbSensor_id` = `ss`.`id`)
            AND (`ss`.`tbNode_id` = `n`.`id`)
            AND (`n`.`tbRoom_id` = `r`.`id`));

-- -----------------------------------------------------
-- View `dh`.`_tbsensortype`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_tbsensortype`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_tbsensortype` AS
    SELECT 
        `a`.`id` AS `id`, `b`.`wording` AS `wording`
    FROM
        (`tbsensortype` `a`
        JOIN `vwtranslation` `b`)
    WHERE
        (`a`.`tbTranslation_id` = `b`.`id`);

-- -----------------------------------------------------
-- View `dh`.`_tbstatus`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_tbstatus`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_tbstatus` AS
    SELECT 
        `a`.`id` AS `id`, `b`.`wording` AS `wording`
    FROM
        (`tbstatus` `a`
        JOIN `vwtranslation` `b`)
    WHERE
        (`a`.`tbTranslation_id` = `b`.`id`);

-- -----------------------------------------------------
-- View `dh`.`_tbstatus`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_tbstatus`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_tbstatus` AS
    SELECT 
        `a`.`id` AS `id`, `b`.`wording` AS `wording`
    FROM
        (`tbstatus` `a`
        JOIN `vwtranslation` `b`)
    WHERE
        (`a`.`tbTranslation_id` = `b`.`id`);

-- -----------------------------------------------------
-- View `dh`.`_tbtriggertype`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_tbtriggertype`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_tbtriggertype` AS
    SELECT 
        `a`.`id` AS `id`, `b`.`wording` AS `wording`
    FROM
        (`tbtriggertype` `a`
        JOIN `vwtranslation` `b`)
    WHERE
        (`a`.`tbTranslation_id` = `b`.`id`)
;

-- -----------------------------------------------------
-- View `dh`.`_tbactuatortype`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_tbactuatortype`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_tbactuatortype` AS
    SELECT 
        `a`.`id` AS `id`, `b`.`wording` AS `wording`
    FROM
        (`tbactuatortype` `a`
        JOIN `vwtranslation` `b`)
    WHERE
        (`a`.`tbTranslation_id` = `b`.`id`)
;

-- -----------------------------------------------------
-- View `dh`.`_tbchart`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_tbchart`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_tbchart` AS
    select 
        `a`.`id` AS `id`, `b`.`wording` AS `wording`
    from
        (`tbchart` `a`
        join `vwtranslation` `b`)
    where
        (`a`.`tbTranslation_id` = `b`.`id`)
;

-- -----------------------------------------------------
-- View `dh`.`_tbnodetype`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_tbnodetype`;
USE `dh`;
CREATE 
 OR REPLACE VIEW `_tbnodetype` AS
    SELECT 
        `a`.`id` AS `id`, `b`.`wording` AS `wording`
    FROM
        (`tbnodetype` `a`
        JOIN `vwtranslation` `b`)
    WHERE
        (`a`.`tbTranslation_id` = `b`.`id`)
;

-- -----------------------------------------------------
-- View `dh`.`vwtriggersensordetails`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtriggersensordetails`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwtriggersensordetails` AS
    SELECT 
        `t`.`id` AS `id`,
        `ss`.`id` AS `sensor_id`,
        `el`.`valmin` AS `valuemin`,
        `el`.`valmax` AS `valuemax`,
        `t`.`currentstatus` AS `currentstatus`,
        `a`.`fadingtime` AS `fadingtime`,
        `a`.`temptime` AS `temptime`,
        `t`.`tbStatus_id` AS `tbStatus_id`,
        `tt`.`icon` AS `trigger_type`,
        GETSTATUSICON(`t`.`currentstatus`) AS `currentstatus_icon`,
        `a`.`tbNode_id` AS `node`,
        `n`.`descr` AS `node_name`,
        `g`.`descr` AS `action_group`,
        `a`.`pinnumber` AS `actuator_pin`,
        `e`.`descr` AS `event`,
        `x`.`descr` AS `timer`,
        `ta`.`icon` AS `actuator_icon`,
        `a`.`descr` AS `actuator`,
        GETSTATUSICON(`a`.`currentstatus`) AS `actuator_currentstatus`,
        `i`.`sequence` AS `sequence`,
        `ct`.`descr` AS `actionon`,
        `cf`.`descr` AS `actionoff`,
        `i`.`id` AS `actionitem_id`,
        `i`.`tbActionGroup_id` AS `actiongroup_id`,
        `a`.`id` AS `actuator_id`
    FROM
        (((((((((((((((`tbevent` `e`
        JOIN `tbtrigger` `t`)
        JOIN `tbactiongroup` `g`)
        JOIN `tbactionitem` `i`)
        JOIN `tbaction` `ct`)
        JOIN `tbaction` `cf`)
        JOIN `tbtriggertype` `y`)
        JOIN `tbtimer` `x`)
        JOIN `tbactuator` `a`)
        JOIN `tbactuatortype` `b`)
        JOIN `vwtranslation` `tr`)
        JOIN `tbsensor` `ss`)
        JOIN `tbeventlogic` `el`)
        JOIN `tbnode` `n`)
        JOIN `tbicon` `tt`)
        JOIN `tbicon` `ta`)
    WHERE
        ((`t`.`tbEvent_id` = `e`.`id`)
            AND (`t`.`tbActionGroup_id` = `g`.`id`)
            AND (`t`.`tbTimer_id` = `x`.`id`)
            AND (`t`.`tbTriggerType_id` = `y`.`id`)
            AND (`g`.`id` = `i`.`tbActionGroup_id`)
            AND (`i`.`tbActuators_id` = `a`.`id`)
            AND (`i`.`actiontrue` = `ct`.`id`)
            AND (`i`.`actionfalse` = `cf`.`id`)
            AND (`a`.`tbActuatorType_id` = `b`.`id`)
            AND (`b`.`tbTranslation_id` = `tr`.`id`)
            AND (`el`.`tbSensor_id` = `ss`.`id`)
            AND (`e`.`id` = `el`.`tbEvent_id`)
            AND (`ss`.`tbNode_id` = `n`.`id`)
            AND (`y`.`tbIcon_id` = `tt`.`id`)
            AND (`b`.`tbIcon_id` = `ta`.`id`))
    ORDER BY `t`.`id` , `g`.`id` , `i`.`id` , `i`.`sequence`;

-- -----------------------------------------------------
-- View `dh`.`vwtriggertitle`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtriggertitle`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwtriggertitle` AS
    select 
        `t`.`id` AS `id`,
        `i`.`icon` AS `trigger_type`,
        getstatusicon(`t`.`currentstatus`) AS `currentstatus_icon`,
        `ag`.`descr` AS `triggerTitle`,
        `ti`.`descr` AS `timer`,
        getstatusicon(`ti`.`currentstatus`) AS `timer_currentstatus`,
        `t`.`tbStatus_id` AS `trigger_status`,
        `t`.`sendalert` AS `sendalert`,
        `ag`.`id` AS `actiongroup_id`
    from
        ((((`tbtrigger` `t`
        join `tbtriggertype` `tt`)
        join `tbactiongroup` `ag`)
        join `tbicon` `i`)
        join `tbtimer` `ti`)
    where
        ((`t`.`tbTriggerType_id` = `tt`.`id`)
            and (`t`.`tbActionGroup_id` = `ag`.`id`)
            and (`tt`.`tbIcon_id` = `i`.`id`)
            and (`t`.`tbTimer_id` = `ti`.`id`));

-- -----------------------------------------------------
-- View `dh`.`vwtriggertitle`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtriggertitle`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwtriggertitle` AS
    select 
        `t`.`id` AS `id`,
        `i`.`icon` AS `trigger_type`,
        getstatusicon(`t`.`currentstatus`) AS `currentstatus_icon`,
        `ag`.`descr` AS `triggerTitle`,
        `ti`.`descr` AS `timer`,
        getstatusicon(`ti`.`currentstatus`) AS `timer_currentstatus`,
        `t`.`tbStatus_id` AS `trigger_status`,
        `t`.`sendalert` AS `sendalert`,
        `ag`.`id` AS `actiongroup_id`
    from
        ((((`tbtrigger` `t`
        join `tbtriggertype` `tt`)
        join `tbactiongroup` `ag`)
        join `tbicon` `i`)
        join `tbtimer` `ti`)
    where
        ((`t`.`tbTriggerType_id` = `tt`.`id`)
            and (`t`.`tbActionGroup_id` = `ag`.`id`)
            and (`tt`.`tbIcon_id` = `i`.`id`)
            and (`t`.`tbTimer_id` = `ti`.`id`));

-- -----------------------------------------------------
-- View `dh`.`vwactuatorlist`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwactuatorlist`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwactuatorlist` AS
    select 
        `a`.`id` AS `id`,
        `a`.`descr` AS `actuator`,
        `n`.`descr` AS `node`
    from
        (`tbactuator` `a`
        join `tbnode` `n`)
    where
        (`a`.`tbNode_id` = `n`.`id`)
    order by `n`.`descr` , `a`.`descr`;

-- -----------------------------------------------------
-- View `dh`.`vwsensorslist`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwsensorslist`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwsensorslist` AS
    SELECT 
        `s`.`id` AS `id`,
        `n`.`descr` AS `node`,
        `s`.`descr` AS `sensor`,
        `s`.`tbChart_id` AS `chart`,
        `r`.`descr` AS `room`
    FROM
        ((`tbsensor` `s`
        JOIN `tbnode` `n`)
        JOIN `tbroom` `r`)
    WHERE
        ((`s`.`tbNode_id` = `n`.`id`)
            AND (`n`.`id` <> 1)
            AND (`n`.`tbRoom_id` = `r`.`id`)
            AND (`s`.`tbStatus_id` <> 0))
    ORDER BY `n`.`descr` , `s`.`descr`;

-- -----------------------------------------------------
-- View `dh`.`vwtriggersall`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtriggersall`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwtriggersall` AS
    SELECT 
        `t`.`id` AS `id`,
        `ss`.`id` AS `sensor_id`,
        `el`.`valmin` AS `valuemin`,
        `el`.`valmax` AS `valuemax`,
        `t`.`currentstatus` AS `currentstatus`,
        `a`.`fadingtime` AS `fadingtime`,
        `a`.`temptime` AS `temptime`,
        `t`.`tbStatus_id` AS `tbStatus_id`,
        `tt`.`icon` AS `trigger_type`,
        `ta`.`icon` AS `actuator_icon`,
        GETSTATUSICON(`t`.`currentstatus`) AS `currentstatus_icon`,
        `a`.`tbNode_id` AS `node`
    FROM
        (((((((((((((((`tbevent` `e`
        JOIN `tbtrigger` `t`)
        JOIN `tbactiongroup` `g`)
        JOIN `tbactionitem` `i`)
        JOIN `tbaction` `ct`)
        JOIN `tbaction` `cf`)
        JOIN `tbtriggertype` `y`)
        JOIN `tbtimer` `x`)
        JOIN `tbactuator` `a`)
        JOIN `tbactuatortype` `b`)
        JOIN `vwtranslation` `tr`)
        JOIN `tbsensor` `ss`)
        JOIN `tbeventlogic` `el`)
        JOIN `tbnode` `n`)
        JOIN `tbicon` `tt`)
        JOIN `tbicon` `ta`)
    WHERE
        ((`t`.`tbEvent_id` = `e`.`id`)
            AND (`t`.`tbActionGroup_id` = `g`.`id`)
            AND (`t`.`tbTimer_id` = `x`.`id`)
            AND (`t`.`tbTriggerType_id` = `y`.`id`)
            AND (`g`.`id` = `i`.`tbActionGroup_id`)
            AND (`i`.`tbActuators_id` = `a`.`id`)
            AND (`i`.`actiontrue` = `ct`.`id`)
            AND (`i`.`actionfalse` = `cf`.`id`)
            AND (`a`.`tbActuatorType_id` = `b`.`id`)
            AND (`b`.`tbTranslation_id` = `tr`.`id`)
            AND (`el`.`tbSensor_id` = `ss`.`id`)
            AND (`e`.`id` = `el`.`tbEvent_id`)
            AND (`ss`.`tbNode_id` = `n`.`id`)
            AND (`y`.`tbIcon_id` = `tt`.`id`)
            AND (`ss`.`tbStatus_id` = 1)
            AND (`n`.`tbStatus_id` = 1)
            AND (`b`.`tbIcon_id` = `ta`.`id`))
    ORDER BY `t`.`id` , `g`.`id` , `i`.`id` , `i`.`sequence`;

-- -----------------------------------------------------
-- View `dh`.`_vwsystems`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_vwsystems`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_vwsystems` AS
    SELECT 
        `sys`.`id` AS `id`,
        `sys`.`descr` AS `system`,
        `ico`.`icon` AS `icon`
    FROM
        (`tbsystem` `sys`
        JOIN `tbicon` `ico`)
    WHERE
        ((`sys`.`tbicon_id` = `ico`.`id`)
            AND (`sys`.`tbstatus_id` = 1));

-- -----------------------------------------------------
-- View `dh`.`_vwsubsystems`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_vwsubsystems`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_vwsubsystems` AS
    SELECT 
        `s`.`id` AS `system_id`,
        `s`.`descr` AS `system`,
        `s`.`users_user_id` AS `user`,
        `sb`.`id` AS `subsystem_id`,
        `sb`.`descr` AS `subsystem`,
        `ss`.`id` AS `sensor_id`,
        `ss`.`descr` AS `sensor`,
        `vsd`.`id` AS `id`,
        `vsd`.`valuemin` AS `valuemin`,
        `vsd`.`valuemax` AS `valuemax`,
        `vsd`.`currentstatus` AS `currentstatus`,
        `vsd`.`fadingtime` AS `fadingtime`,
        `vsd`.`temptime` AS `temptime`,
        `vsd`.`tbStatus_id` AS `tbStatus_id`,
        `vsd`.`trigger_type` AS `trigger_type`,
        `vsd`.`currentstatus_icon` AS `currentstatus_icon`,
        `vsd`.`node` AS `node`,
        `vsd`.`node_name` AS `node_name`,
        `vsd`.`action_group` AS `action_group`,
        `vsd`.`actuator_pin` AS `actuator_pin`,
        `vsd`.`event` AS `event`,
        `vsd`.`timer` AS `timer`,
        `vsd`.`actuator_icon` AS `actuator_icon`,
        `vsd`.`actuator` AS `actuator`,
        `vsd`.`actuator_currentstatus` AS `actuator_currentstatus`,
        `vsd`.`sequence` AS `sequence`,
        `vsd`.`actionon` AS `actionon`,
        `vsd`.`actionoff` AS `actionoff`,
        `vsd`.`actionitem_id` AS `actionitem_id`
    FROM
        ((((`tbsystem` `s`
        JOIN `tbsubsystem` `sb`)
        JOIN `tbsubsystemsensor` `sbs`)
        JOIN `tbsensor` `ss`)
        LEFT JOIN `vwtriggersensordetails` `vsd` ON ((`ss`.`id` = `vsd`.`sensor_id`)))
    WHERE
        ((`s`.`id` = `sb`.`tbSystem_id`)
            AND (`sb`.`id` = `sbs`.`tbSubsystem_id`)
            AND (`sbs`.`tbsensor_id` = `ss`.`id`));

-- -----------------------------------------------------
-- View `dh`.`_tbmenu`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_tbmenu`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_tbmenu` AS
    SELECT 
        `m`.`page` AS `page`,
        `c`.`icon` AS `icon`,
        `t`.`wording` AS `menuitem`
    FROM
        ((`tbmenu` `m`
        JOIN `tbicon` `c`)
        JOIN `vwtranslation` `t`)
    WHERE
        ((`m`.`tbicon_id` = `c`.`id`)
            AND (`m`.`tbtranslation_id` = `t`.`id`));

-- -----------------------------------------------------
-- View `dh`.`_vwsubsystems`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_vwsubsystems`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_vwsubsystems` AS
    SELECT 
        `s`.`id` AS `system_id`,
        `s`.`descr` AS `system`,
        `s`.`users_user_id` AS `user`,
        `sb`.`id` AS `subsystem_id`,
        `sb`.`descr` AS `subsystem`,
        `ss`.`id` AS `sensor_id`,
        `ss`.`descr` AS `sensor`,
        `vsd`.`id` AS `id`,
        `vsd`.`valuemin` AS `valuemin`,
        `vsd`.`valuemax` AS `valuemax`,
        `vsd`.`currentstatus` AS `currentstatus`,
        `vsd`.`fadingtime` AS `fadingtime`,
        `vsd`.`temptime` AS `temptime`,
        `vsd`.`tbStatus_id` AS `tbStatus_id`,
        `vsd`.`trigger_type` AS `trigger_type`,
        `vsd`.`currentstatus_icon` AS `currentstatus_icon`,
        `vsd`.`node` AS `node`,
        `vsd`.`node_name` AS `node_name`,
        `vsd`.`action_group` AS `action_group`,
        `vsd`.`actuator_pin` AS `actuator_pin`,
        `vsd`.`event` AS `event`,
        `vsd`.`timer` AS `timer`,
        `vsd`.`actuator_icon` AS `actuator_icon`,
        `vsd`.`actuator` AS `actuator`,
        `vsd`.`actuator_currentstatus` AS `actuator_currentstatus`,
        `vsd`.`sequence` AS `sequence`,
        `vsd`.`actionon` AS `actionon`,
        `vsd`.`actionoff` AS `actionoff`,
        `vsd`.`actionitem_id` AS `actionitem_id`
    FROM
        ((((`tbsystem` `s`
        JOIN `tbsubsystem` `sb`)
        JOIN `tbsubsystemsensor` `sbs`)
        JOIN `tbsensor` `ss`)
        LEFT JOIN `vwtriggersensordetails` `vsd` ON ((`ss`.`id` = `vsd`.`sensor_id`)))
    WHERE
        ((`s`.`id` = `sb`.`tbSystem_id`)
            AND (`sb`.`id` = `sbs`.`tbSubsystem_id`)
            AND (`sbs`.`tbsensor_id` = `ss`.`id`));

-- -----------------------------------------------------
-- View `dh`.`vwsubsystemactuators`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwsubsystemactuators`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwsubsystemactuators` AS
    SELECT 
        `ss`.`tbSubsystem_id` AS `subsystem_id`,
        `td`.`sensor_id` AS `sensor_id`,
        `da`.`id` AS `id`,
        `da`.`tbActuatorType_id` AS `tbActuatorType_id`,
        `da`.`tbNode_id` AS `tbNode_id`,
        `da`.`descr` AS `descr`,
        `da`.`pinnumber` AS `pinnumber`,
        `da`.`currentstatus` AS `currentstatus`,
        `da`.`currentstatusicon` AS `currentstatusicon`,
        `da`.`currentvalue` AS `currentvalue`,
        `da`.`status` AS `status`,
        `da`.`tbOutput_id` AS `tbOutput_id`,
        `da`.`valueon` AS `valueon`,
        `da`.`valueoff` AS `valueoff`,
        `da`.`temptime` AS `temptime`,
        `da`.`fadingtime` AS `fadingtime`,
        `da`.`actuatortypeicon` AS `actuatortypeicon`,
        `da`.`actuatortypecolor` AS `actuatortypecolor`
    FROM
        ((`vwtriggersdashboard` `td`
        JOIN `vwdashboardactuators` `da`)
        JOIN `tbsubsystemsensor` `ss`)
    WHERE
        ((`td`.`actuator_id` = `da`.`id`)
            AND (`ss`.`tbsensor_id` = `td`.`sensor_id`));

-- -----------------------------------------------------
-- View `dh`.`_vwfunctions`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`_vwfunctions`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `_vwfunctions` AS
    SELECT 
        `fn`.`id` AS `id`,
        `fn`.`descr` AS `function`,
        `ico`.`icon` AS `icon`,
        GETSTATUSICON(`fn`.`currentstatus`) AS `currentstatusicon`,
        `fn`.`currentstatus` AS `currentstatus`
    FROM
        (`tbfunction` `fn`
        JOIN `tbicon` `ico`)
    WHERE
        (`fn`.`tbicon_id` = `ico`.`id`);

-- -----------------------------------------------------
-- View `dh`.`vwtriggerfunctiontitle`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtriggerfunctiontitle`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwtriggerfunctiontitle` AS
    SELECT 
        `t`.`id` AS `id`,
        `i`.`icon` AS `trigger_type`,
        GETSTATUSICON(`t`.`currentstatus`) AS `currentstatus_icon`,
        `ag`.`descr` AS `triggerTitle`,
        `ti`.`descr` AS `timer`,
        GETSTATUSICON(`ti`.`currentstatus`) AS `timer_currentstatus`,
        `t`.`tbStatus_id` AS `trigger_status`,
        `t`.`sendalert` AS `sendalert`,
        `ag`.`id` AS `actiongroup_id`,
        `ag`.`descr` AS `actiongroup`,
        `f`.`id` AS `function_id`,
        `f`.`descr` AS `function`,
        GETSTATUSICON(`f`.`currentstatus`) AS `function_currentstatus`,
        `e`.`descr` AS `event`,
        `e`.`event_status` AS `event_status`
    FROM
        (((((((`tbtrigger` `t`
        JOIN `tbtriggertype` `tt`)
        JOIN `tbactiongroup` `ag`)
        JOIN `tbicon` `i`)
        JOIN `tbtimer` `ti`)
        JOIN `tbfunction` `f`)
        JOIN `tbfunctiontrigger` `ft`)
        JOIN `vwevents` `e`)
    WHERE
        ((`t`.`tbTriggerType_id` = `tt`.`id`)
            AND (`t`.`tbActionGroup_id` = `ag`.`id`)
            AND (`tt`.`tbIcon_id` = `i`.`id`)
            AND (`t`.`tbTimer_id` = `ti`.`id`)
            AND (`ft`.`tbtrigger_id` = `t`.`id`)
            AND (`ft`.`tbfunction_id` = `f`.`id`)
            AND (`t`.`tbEvent_id` = `e`.`id`));

-- -----------------------------------------------------
-- View `dh`.`vwtriggerfunctionall`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dh`.`vwtriggerfunctionall`;
USE `dh`;
CREATE 
     OR REPLACE ALGORITHM = UNDEFINED 
    DEFINER = `root`@`localhost` 
    SQL SECURITY DEFINER
VIEW `vwtriggerfunctionall` AS
    SELECT 
        `t`.`id` AS `id`,
        `ss`.`id` AS `sensor_id`,
        `el`.`valmin` AS `valuemin`,
        `el`.`valmax` AS `valuemax`,
        `t`.`currentstatus` AS `currentstatus`,
        `a`.`fadingtime` AS `fadingtime`,
        `a`.`temptime` AS `temptime`,
        `t`.`tbStatus_id` AS `tbStatus_id`,
        `tt`.`icon` AS `trigger_type`,
        `ta`.`icon` AS `actuator_icon`,
        GETSTATUSICON(`a`.`currentstatus`) AS `currentstatus_icon`,
        `a`.`tbNode_id` AS `node`,
        `fn`.`id` AS `function_id`,
        `ri`.`icon` AS `room`,
        `r`.`descr` AS `roomtitle`,
        `g`.`descr` AS `triggertitle`
    FROM
        (((((((((((((((((((`tbevent` `e`
        JOIN `tbtrigger` `t`)
        JOIN `tbactiongroup` `g`)
        JOIN `tbactionitem` `i`)
        JOIN `tbaction` `ct`)
        JOIN `tbaction` `cf`)
        JOIN `tbtriggertype` `y`)
        JOIN `tbtimer` `x`)
        JOIN `tbactuator` `a`)
        JOIN `tbactuatortype` `b`)
        JOIN `vwtranslation` `tr`)
        JOIN `tbsensor` `ss`)
        JOIN `tbeventlogic` `el`)
        JOIN `tbnode` `n`)
        JOIN `tbroom` `r`)
        JOIN `tbicon` `ri`)
        JOIN `tbicon` `tt`)
        JOIN `tbicon` `ta`)
        JOIN `tbfunction` `fn`)
        JOIN `tbfunctiontrigger` `ft`)
    WHERE
        ((`t`.`tbEvent_id` = `e`.`id`)
            AND (`t`.`tbActionGroup_id` = `g`.`id`)
            AND (`t`.`tbTimer_id` = `x`.`id`)
            AND (`t`.`tbTriggerType_id` = `y`.`id`)
            AND (`g`.`id` = `i`.`tbActionGroup_id`)
            AND (`i`.`tbActuators_id` = `a`.`id`)
            AND (`i`.`actiontrue` = `ct`.`id`)
            AND (`i`.`actionfalse` = `cf`.`id`)
            AND (`a`.`tbActuatorType_id` = `b`.`id`)
            AND (`b`.`tbTranslation_id` = `tr`.`id`)
            AND (`el`.`tbSensor_id` = `ss`.`id`)
            AND (`e`.`id` = `el`.`tbEvent_id`)
            AND (`ss`.`tbNode_id` = `n`.`id`)
            AND (`y`.`tbIcon_id` = `tt`.`id`)
            AND (`ss`.`tbStatus_id` = 1)
            AND (`n`.`tbStatus_id` = 1)
            AND (`b`.`tbIcon_id` = `ta`.`id`)
            AND (`fn`.`id` = `ft`.`tbfunction_id`)
            AND (`ft`.`tbtrigger_id` = `t`.`id`)
            AND (`n`.`tbRoom_id` = `r`.`id`)
            AND (`r`.`tbicon_id` = `ri`.`id`))
    ORDER BY `t`.`id` , `g`.`id` , `i`.`id` , `i`.`sequence`;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `dh`.`tbaction`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbaction` (`id`, `descr`, `value`) VALUES (0, 'OFF', 0);
INSERT INTO `dh`.`tbaction` (`id`, `descr`, `value`) VALUES (1, 'ON', 1);
INSERT INTO `dh`.`tbaction` (`id`, `descr`, `value`) VALUES (2, 'OFF Servo', 0);
INSERT INTO `dh`.`tbaction` (`id`, `descr`, `value`) VALUES (3, 'ON Servo', 180);
INSERT INTO `dh`.`tbaction` (`id`, `descr`, `value`) VALUES (4, 'OFF Analog', 0);
INSERT INTO `dh`.`tbaction` (`id`, `descr`, `value`) VALUES (5, 'ON Analog', 255);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbactiongroup`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (1, 'Scatta allarme 1');
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (2, 'Scatta allarme 2');
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (3, 'Scatta allarme 3');
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (4, 'Scatta allarme 4');
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (5, 'Scatta allarme 5');
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (6, 'Suona buzzer');
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (7, 'Suona buzzer');
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (8, 'Suona buzzer');
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (9, 'Suona buzzer');
INSERT INTO `dh`.`tbactiongroup` (`id`, `descr`) VALUES (10, 'Suona buzzer');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbcolor`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (1, '#6994ce');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (2, '#19175a');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (3, '#c24b04');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (4, '#67b600');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (5, '#392a38');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (6, '#c37abf');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (7, '#930d0d');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (8, '#66666c');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (9, '#000');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (10, '#F00');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (11, '#c37abf');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (12, '#66633c');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (13, '#a71d1d');
INSERT INTO `dh`.`tbcolor` (`id`, `hexCode`) VALUES (14, '#b776ea');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbicon`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (1, 'e008', 'sts_active');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (2, 'e01a', 'sen_gas');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (3, 'e02b', 'sen_humidity');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (4, 'e036', 'sen_light');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (5, 'e03d', 'sen_motion');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (6, 'e013', 'sen_noise');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (7, 'e028', 'sen_power');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (8, 'e025', 'sen_pressure');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (9, 'e039', 'sen_temperature');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (10, 'e034', 'act_buzzer');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (11, 'e022', 'act_ceiling');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (12, 'e024', 'act_key');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (13, 'e020', 'act_bulb');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (14, 'e00e', 'act_servo');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (15, 'e038', 'act_relay');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (16, 'e006', 'act_camera');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (17, 'e029', 'act_plug');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (18, 'e03f', 'nod_xbee');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (19, 'e041', 'nod_internal');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (20, 'e00c', 'nod_external');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (21, 'e00d', 'trg_local');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (22, 'e02a', 'trg_remoteperm');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (23, 'e00a', 'trg_remotetemp');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (24, 'e037', 'trg_flipflop');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (25, 'e010', 'trg_timer');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (26, 'e00f', 'sts_inactive');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (27, 'e00b', 'sts_suspended');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (28, 'e02b', 'crt_humidity');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (29, 'e036', 'crt_light');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (30, 'e039', 'crt_temperature');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (31, 'e012', 'fn_away');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (32, 'e02d', 'fn_athome');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (33, 'e03c', 'sys_alarm');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (34, 'e01b', 'sys_acquaria');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (35, 'e01e', 'sys_heating');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (36, 'e027', 'mn_system');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (37, 'e03e', 'mn_function');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (38, 'e018', 'mn_favourite');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (39, 'e015', 'act_ciabatta');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (40, 'e026', 'act_door');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (41, 'e019', 'act_fire');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (42, 'e031', 'act_switch');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (43, 'e021', 'act_tablelamp');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (44, 'e040', 'act_window');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (45, 'e000', 'arc_airfresh');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (46, 'e002', 'fn_night');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (47, 'e01d', 'hea_actuator');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (48, 'e003', 'hea_back');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (49, 'e001', 'hea_chart');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (50, 'e01f', 'hea_home');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (51, 'e017', 'hea_node');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (52, 'e042', 'hea_option');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (53, 'e02f', 'hea_settings');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (54, 'e014', 'oth_fan');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (55, 'e004', 'oth_mail');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (56, 'e023', 'oth_menu');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (57, 'e035', 'oth_step');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (58, 'e01c', 'oth_stop');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (59, 'e030', 'roo_bathroom');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (60, 'e02c', 'roo_bedroom');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (61, 'e016', 'roo_corridor');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (62, 'e011', 'roo_dining');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (63, 'e009', 'roo_kitchen');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (64, 'e033', 'roo_living');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (65, 'e02e', 'roo_roof');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (66, 'e005', 'roo_stairs');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (67, 'e007', 'sen_pollution');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (68, 'e032', 'trg_actionoff');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (69, 'e03a', 'trg_actionon');
INSERT INTO `dh`.`tbicon` (`id`, `icon`, `descr`) VALUES (70, 'e03b', 'trg_event');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tboutput`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tboutput` (`id`, `descr`) VALUES (0, 'Digital');
INSERT INTO `dh`.`tboutput` (`id`, `descr`) VALUES (1, 'Analog');
INSERT INTO `dh`.`tboutput` (`id`, `descr`) VALUES (2, 'Servo');
INSERT INTO `dh`.`tboutput` (`id`, `descr`) VALUES (3, 'not used');
INSERT INTO `dh`.`tboutput` (`id`, `descr`) VALUES (4, 'Tone');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbtranslation`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (1);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (2);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (3);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (4);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (5);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (6);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (7);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (8);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (9);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (10);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (11);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (12);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (13);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (14);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (15);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (16);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (17);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (18);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (19);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (20);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (21);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (22);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (23);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (24);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (25);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (26);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (27);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (28);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (29);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (30);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (31);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (32);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (33);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (34);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (35);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (36);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (37);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (38);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (39);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (40);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (41);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (42);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (43);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (44);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (100);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (101);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (102);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (103);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (104);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (45);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (46);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (200);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (201);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (202);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (47);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (48);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (49);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (50);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (51);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (52);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (53);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (54);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (55);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (56);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (57);
INSERT INTO `dh`.`tbtranslation` (`id`) VALUES (58);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbactuatortype`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (1, 5, 13, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (2, 6, 11, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (3, 7, 15, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (4, 8, 10, 9, 4, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (5, 9, 16, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (6, 10, 12, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (7, 11, 17, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (8, 12, 14, 9, 2, 255, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (9, 13, 13, 9, 1, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (10, 43, 26, 13, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (11, 47, 39, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (12, 48, 40, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (13, 49, 42, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (14, 50, 43, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (15, 51, 44, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (16, 52, 45, 9, 0, 1, 0);
INSERT INTO `dh`.`tbactuatortype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `tbOutput_id`, `valueon`, `valueoff`) VALUES (17, 53, 54, 9, 0, 1, 0);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbnodetype`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbnodetype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `note`) VALUES (0, 14, 20, 10, 0, 100, 'esterno');
INSERT INTO `dh`.`tbnodetype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `note`) VALUES (1, 15, 19, 9, 0, 100, 'local');
INSERT INTO `dh`.`tbnodetype` (`id`, `tbTranslation_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `note`) VALUES (2, 16, 18, 2, 0, 100, 'xbee');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`users`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`users` (`user_id`, `user_name`, `user_password_hash`, `user_email`, `user_active`, `user_activation_hash`, `user_password_reset_hash`, `user_password_reset_timestamp`, `user_rememberme_token`, `user_failed_logins`, `user_last_failed_login`, `user_registration_datetime`, `user_registration_ip`, `userscol`, `user_language`) VALUES (1, 'franco', '$2y$10$9O5ZR4ixDXkigCPcXBhOw.Lqd7ENqLUiwVaiHX3XV.4zIlftPbN4y', 'franco.paren@gmail.com', 1, '41d4c1be9928ef8634e1d6f92bb25e8ebef67c1a', NULL, NULL, '6d9666fe99579ce290c4680535dc7550bb403a611d3da6f18aee086d7eb120de', 0, NULL, '2015-01-14 16:51:15', '127.0.0.1', NULL, 'EN');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tblocation`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tblocation` (`id`, `descr`, `users_user_id`) VALUES (1, 'CasaMia', 1);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbroom`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (1, 1, 'Studio', 64);
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (2, 1, 'Kitchen', 63);
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (3, 1, 'Living', 64);
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (4, 1, 'Dining', 62);
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (5, 1, 'Bedroom', 60);
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (6, 1, 'Bathroom', 59);
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (7, 1, 'Roof', 65);
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (8, 1, 'Entrance', 61);
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (9, 1, 'Stairs', 66);
INSERT INTO `dh`.`tbroom` (`id`, `tbLocation_id`, `descr`, `tbicon_id`) VALUES (10, 1, 'External', 20);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbstatus`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbstatus` (`id`, `tbIcon_id`, `tbTranslation_id`) VALUES (0, 26, 43);
INSERT INTO `dh`.`tbstatus` (`id`, `tbIcon_id`, `tbTranslation_id`) VALUES (1, 1, 1);
INSERT INTO `dh`.`tbstatus` (`id`, `tbIcon_id`, `tbTranslation_id`) VALUES (2, 27, 44);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbnode`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbnode` (`id`, `descr`, `tbNodeType_id`, `tbStatus_id`, `xbee_low_address`, `xbee_high_address`, `signalvalue`, `currentstatus`, `lastupdate`, `tbRoom_id`) VALUES (1, 'dummy', 1, 1, NULL, NULL, 100, 1, '2015-01-01 12:00:00', 1);
INSERT INTO `dh`.`tbnode` (`id`, `descr`, `tbNodeType_id`, `tbStatus_id`, `xbee_low_address`, `xbee_high_address`, `signalvalue`, `currentstatus`, `lastupdate`, `tbRoom_id`) VALUES (2, 'Esterno', 0, 1, NULL, NULL, 0, 1, '2015-01-01 12:00:00', 10);
INSERT INTO `dh`.`tbnode` (`id`, `descr`, `tbNodeType_id`, `tbStatus_id`, `xbee_low_address`, `xbee_high_address`, `signalvalue`, `currentstatus`, `lastupdate`, `tbRoom_id`) VALUES (3, 'Studio', 2, 1, 1087807222, 1286656, 0, 1, '2015-01-01 12:00:00', 1);
INSERT INTO `dh`.`tbnode` (`id`, `descr`, `tbNodeType_id`, `tbStatus_id`, `xbee_low_address`, `xbee_high_address`, `signalvalue`, `currentstatus`, `lastupdate`, `tbRoom_id`) VALUES (4, 'Salotto', 2, 1, 1087807235, 1286656, 0, 1, '2015-01-01 12:00:00', 3);
INSERT INTO `dh`.`tbnode` (`id`, `descr`, `tbNodeType_id`, `tbStatus_id`, `xbee_low_address`, `xbee_high_address`, `signalvalue`, `currentstatus`, `lastupdate`, `tbRoom_id`) VALUES (5, 'Tinello', 2, 1, 1087807224, 1286656, 0, 1, '2015-01-01 12:00:00', 4);
INSERT INTO `dh`.`tbnode` (`id`, `descr`, `tbNodeType_id`, `tbStatus_id`, `xbee_low_address`, `xbee_high_address`, `signalvalue`, `currentstatus`, `lastupdate`, `tbRoom_id`) VALUES (6, 'Corridoio', 2, 1, 1087807178, 1286656, 0, 1, '2015-01-01 12:00:00', 1);
INSERT INTO `dh`.`tbnode` (`id`, `descr`, `tbNodeType_id`, `tbStatus_id`, `xbee_low_address`, `xbee_high_address`, `signalvalue`, `currentstatus`, `lastupdate`, `tbRoom_id`) VALUES (7, 'Cucina', 2, 1, 1087807180, 1286656, 0, 1, '2015-01-01 12:00:00', 2);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbactuator`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (5, 3, 4, 'Rele 10 - 4', 10, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (6, 4, 4, 'Buzzer 4', 8, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (7, 8, 4, 'Servo', 9, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (8, 3, 4, 'Rele 11 - 4', 11, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (9, 3, 4, 'Rele 14 - 4', 14, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (1, 3, 3, 'Rele 11 - 2', 11, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (2, 4, 3, 'Buzzer 3', 8, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (10, 4, 5, 'Buzzer 5', 8, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (11, 3, 5, 'Rele 10 - 5', 10, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (12, 3, 5, 'Rele 11 - 5', 11, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (13, 3, 5, 'Rele 14 - 5', 14, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (3, 3, 3, 'Rele 14 - 3', 14, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (4, 3, 3, 'Rele 10 - 3', 10, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (14, 4, 6, 'Buzzer 6', 8, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (18, 4, 7, 'Buzzer 7', 8, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (16, 3, 6, 'Rele 10 - 6', 10, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (19, 3, 7, 'Rele 10 - 7', 10, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (15, 3, 6, 'Rele 11 - 6', 11, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (20, 3, 7, 'Rele 11 - 7', 11, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (17, 3, 6, 'Rele 14 - 6', 14, 1, 0, 0, 0);
INSERT INTO `dh`.`tbactuator` (`id`, `tbActuatorType_id`, `tbNode_id`, `descr`, `pinnumber`, `currentstatus`, `currentvalue`, `fadingtime`, `temptime`) VALUES (21, 3, 7, 'Rele 14 - 7', 14, 1, 0, 0, 0);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbactionitem`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (1, 1, 1, 1, 0, 1);
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (2, 2, 8, 1, 0, 1);
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (3, 3, 12, 1, 0, 1);
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (4, 4, 15, 1, 0, 1);
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (5, 5, 20, 1, 0, 1);
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (6, 6, 18, 1, 0, 1);
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (7, 7, 6, 1, 0, 1);
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (8, 8, 10, 1, 0, 1);
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (9, 9, 14, 1, 0, 1);
INSERT INTO `dh`.`tbactionitem` (`id`, `tbActionGroup_id`, `tbActuators_id`, `actiontrue`, `actionfalse`, `sequence`) VALUES (10, 10, 2, 1, 0, 1);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbchart`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbchart` (`id`, `tbTranslation_id`, `tbicon_id`) VALUES (1, 2, 28);
INSERT INTO `dh`.`tbchart` (`id`, `tbTranslation_id`, `tbicon_id`) VALUES (2, 3, 29);
INSERT INTO `dh`.`tbchart` (`id`, `tbTranslation_id`, `tbicon_id`) VALUES (3, 4, 30);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbconvformula`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbconvformula` (`id`, `formula`) VALUES (1, 'f_generic');
INSERT INTO `dh`.`tbconvformula` (`id`, `formula`) VALUES (2, 'f_temp');
INSERT INTO `dh`.`tbconvformula` (`id`, `formula`) VALUES (3, 'f_dht21');
INSERT INTO `dh`.`tbconvformula` (`id`, `formula`) VALUES (4, 'f_analoglux');
INSERT INTO `dh`.`tbconvformula` (`id`, `formula`) VALUES (5, 'f_GA1A12S202');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbevent`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (1, 'Da definire', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (2, 'Da definire', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (3, 'Da definire', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (4, 'Da definire', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (5, 'movimento rilevato', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (6, 'movimento rilevato', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (7, 'fuga di gas rilevata', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (8, 'movimento rilevato', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (9, 'movimento rilevato', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (10, 'movimento rilevato', 0);
INSERT INTO `dh`.`tbevent` (`id`, `descr`, `currentstatus`) VALUES (11, 'movimento rilevato', 0);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbparam`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbparam` (`id`, `pvalue`, `descr`, `ptype`, `pindex`) VALUES (1, 2, 'Timer0', 'T', 0);
INSERT INTO `dh`.`tbparam` (`id`, `pvalue`, `descr`, `ptype`, `pindex`) VALUES (2, 100, 'Timer1', 'T', 1);
INSERT INTO `dh`.`tbparam` (`id`, `pvalue`, `descr`, `ptype`, `pindex`) VALUES (3, 200, 'Timer2', 'T', 2);
INSERT INTO `dh`.`tbparam` (`id`, `pvalue`, `descr`, `ptype`, `pindex`) VALUES (4, 600, 'Timer3', 'T', 3);
INSERT INTO `dh`.`tbparam` (`id`, `pvalue`, `descr`, `ptype`, `pindex`) VALUES (5, 800, 'Timer4', 'T', 4);
INSERT INTO `dh`.`tbparam` (`id`, `pvalue`, `descr`, `ptype`, `pindex`) VALUES (6, 1, 'Initialize', 'I', 0);
INSERT INTO `dh`.`tbparam` (`id`, `pvalue`, `descr`, `ptype`, `pindex`) VALUES (7, 0, 'SendEmail', 'E', 0);
INSERT INTO `dh`.`tbparam` (`id`, `pvalue`, `descr`, `ptype`, `pindex`) VALUES (8, 5, 'values Init strings', 'P', 0);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbum`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbum` (`id`, `descr`) VALUES (1, '°C');
INSERT INTO `dh`.`tbum` (`id`, `descr`) VALUES (2, 'Lux');
INSERT INTO `dh`.`tbum` (`id`, `descr`) VALUES (3, 'Ut');
INSERT INTO `dh`.`tbum` (`id`, `descr`) VALUES (4, '%');
INSERT INTO `dh`.`tbum` (`id`, `descr`) VALUES (5, 'V');
INSERT INTO `dh`.`tbum` (`id`, `descr`) VALUES (6, 'dB');
INSERT INTO `dh`.`tbum` (`id`, `descr`) VALUES (7, 'hPa');
INSERT INTO `dh`.`tbum` (`id`, `descr`) VALUES (8, '.');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbsensortype`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (0, 29, 1, 4, 14, 0, 100, 2, 'dummy sensor type', 8);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (1, 17, 1, 7, 14, 0, 1023, 5, NULL, 5);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (2, 18, 1, 2, 14, 0, 1023, 5, NULL, 3);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (3, 19, 5, 4, 14, 0, 1023, 3, NULL, 2);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (4, 21, 1, 5, 14, 0, 1023, 5, NULL, 2);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (5, 22, 1, 8, 14, 900, 1100, 3, NULL, 3);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (6, 23, 1, 6, 14, 0, 1023, 4, NULL, 6);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (7, 24, 3, 9, 14, 0, 40, 3, NULL, 1);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (8, 25, 2, 9, 14, 0, 40, 3, NULL, 1);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (9, 26, 1, 9, 14, 0, 45, 3, NULL, 1);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (10, 27, 3, 3, 14, 0, 100, 3, NULL, 4);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (11, 28, 1, 3, 14, 0, 100, 3, NULL, 4);
INSERT INTO `dh`.`tbsensortype` (`id`, `tbTranslation_id`, `tbConvFormula_id`, `tbIcon_id`, `tbColor_id`, `scalemin`, `scalemax`, `timer`, `notes`, `tbum_id`) VALUES (12, 58, 1, 67, 14, 0, 1023, 3, NULL, 3);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbsensor`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (1, 'Dummy', 1, 1, 0, 0, 1023, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (2, 'Presence', 3, 1, 4, 21, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (3, 'Temperature', 4, 1, 7, 24, 0, 0, 3, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (4, 'Humidity', 4, 1, 10, 25, 0, 0, 1, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (5, 'Noise', 3, 1, 6, 18, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (6, 'Illumination', 3, 1, 3, 20, 0, 1, 2, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (7, 'Illumination', 4, 1, 3, 20, 0, 0, 2, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (8, 'Temperature', 3, 1, 7, 24, 0, 1, 3, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (9, 'Gas', 7, 1, 2, 22, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (10, 'Movement', 4, 1, 4, 23, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (11, '220V Power', 4, 1, 1, 19, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (12, 'Temperature', 2, 1, 9, 1000, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (13, 'Humidity', 2, 1, 11, 1001, 0, 0, 1, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (14, 'Pressure', 2, 1, 5, 1002, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (16, 'Illumination', 6, 1, 3, 20, 0, 0, 2, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (17, 'Presence', 6, 1, 4, 21, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (18, 'Movement', 6, 1, 4, 23, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (19, 'Temperature', 6, 1, 7, 24, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (20, 'Humidity', 6, 1, 10, 25, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (21, 'Presence', 5, 1, 4, 21, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (22, 'Temperature', 5, 1, 7, 24, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (23, 'Humidity', 3, 1, 10, 25, 0, 0, 1, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (24, 'Temperature', 7, 1, 7, 24, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (25, 'Humidity', 7, 1, 10, 25, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (26, 'Presence', 7, 1, 4, 21, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (27, 'Illumination', 7, 1, 3, 20, 0, 0, 2, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (28, 'Illumination', 5, 1, 3, 20, 0, 0, 2, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (29, 'Noise', 5, 1, 6, 18, 0, 0, NULL, '2015-01-01 00:00:00', 1);
INSERT INTO `dh`.`tbsensor` (`id`, `descr`, `tbNode_id`, `tbStatus_id`, `tbSensorType_id`, `pin_number`, `currentvalue`, `addtohomepage`, `tbChart_id`, `lastupdate`, `currentstatus`) VALUES (30, 'Presence', 4, 1, 4, 21, 0, 0, NULL, '2015-01-01 00:00:00', 1);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbeventlogic`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (1, 1, 8, 24, 40, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (2, 2, 3, 22, 40, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (3, 3, 6, 20, 200, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (4, 4, 7, 40, 100, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (5, 5, 2, 800, 1024, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (6, 6, 10, 1, 100, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (7, 7, 9, 100, 1024, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (8, 8, 17, 800, 1023, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (9, 9, 21, 800, 1023, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (10, 10, 26, 800, 1024, 1, NULL, NULL);
INSERT INTO `dh`.`tbeventlogic` (`id`, `tbEvent_id`, `tbSensor_id`, `valmin`, `valmax`, `sequence`, `logic`, `bracket`) VALUES (11, 11, 30, 800, 1024, 1, NULL, NULL);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tblanguage`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tblanguage` (`id`, `descr`) VALUES ('EN', 'English');
INSERT INTO `dh`.`tblanguage` (`id`, `descr`) VALUES ('IT', 'Italiano');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbsystem`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbsystem` (`id`, `descr`, `users_user_id`, `tbstatus_id`, `tbicon_id`) VALUES (1, 'Alarm', 1, 1, 33);
INSERT INTO `dh`.`tbsystem` (`id`, `descr`, `users_user_id`, `tbstatus_id`, `tbicon_id`) VALUES (2, 'Acquario', 1, 0, 34);
INSERT INTO `dh`.`tbsystem` (`id`, `descr`, `users_user_id`, `tbstatus_id`, `tbicon_id`) VALUES (3, 'Heating system', 1, 1, 35);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbsubsystem`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbsubsystem` (`id`, `descr`, `tbSystem_id`, `tbicon_id`) VALUES (1, 'Clima', 3, 35);
INSERT INTO `dh`.`tbsubsystem` (`id`, `descr`, `tbSystem_id`, `tbicon_id`) VALUES (2, 'Sistema di allarme', 1, 33);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbtimerdaily`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbtimerdaily` (`id`, `descr`, `timestart`, `timeend`) VALUES (1, 'Always', '00:00:00', '24:00:00');
INSERT INTO `dh`.`tbtimerdaily` (`id`, `descr`, `timestart`, `timeend`) VALUES (2, 'Always', '00:00:00', '24:00:00');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbtimermonthly`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbtimermonthly` (`id`, `descr`, `m1`, `m2`, `m3`, `m4`, `m5`, `m6`, `m7`, `m8`, `m9`, `m10`, `m11`, `m12`) VALUES (1, 'Always', 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
INSERT INTO `dh`.`tbtimermonthly` (`id`, `descr`, `m1`, `m2`, `m3`, `m4`, `m5`, `m6`, `m7`, `m8`, `m9`, `m10`, `m11`, `m12`) VALUES (2, 'Winter', 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 1, 1);
INSERT INTO `dh`.`tbtimermonthly` (`id`, `descr`, `m1`, `m2`, `m3`, `m4`, `m5`, `m6`, `m7`, `m8`, `m9`, `m10`, `m11`, `m12`) VALUES (3, 'Summer', 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbtimerweekly`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbtimerweekly` (`id`, `descr`, `wd0`, `wd1`, `wd2`, `wd3`, `wd4`, `wd5`, `wd6`) VALUES (1, 'Always', 1, 1, 1, 1, 1, 1, 1);
INSERT INTO `dh`.`tbtimerweekly` (`id`, `descr`, `wd0`, `wd1`, `wd2`, `wd3`, `wd4`, `wd5`, `wd6`) VALUES (2, 'Working days', 1, 1, 1, 1, 1, 0, 0);
INSERT INTO `dh`.`tbtimerweekly` (`id`, `descr`, `wd0`, `wd1`, `wd2`, `wd3`, `wd4`, `wd5`, `wd6`) VALUES (3, 'Weekend', 0, 0, 0, 0, 0, 1, 1);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbtimer`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbtimer` (`id`, `descr`, `tbTimerDaily_id`, `tbTimerMonthly_id`, `tbTimerWeekly_id`, `currentstatus`) VALUES (1, 'Always', 1, 1, 1, 1);
INSERT INTO `dh`.`tbtimer` (`id`, `descr`, `tbTimerDaily_id`, `tbTimerMonthly_id`, `tbTimerWeekly_id`, `currentstatus`) VALUES (2, 'Working days', 1, 1, 2, 1);
INSERT INTO `dh`.`tbtimer` (`id`, `descr`, `tbTimerDaily_id`, `tbTimerMonthly_id`, `tbTimerWeekly_id`, `currentstatus`) VALUES (3, 'Festivi', 1, 1, 3, 0);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbtranslationitem`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 1, 'Active');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 2, 'Humidity');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 3, 'Light');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 4, 'Temp');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 5, 'Lamp');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 6, 'Ceiling lamp');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 7, 'Relay');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 8, 'Buzzer');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 9, 'Camera');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 10, 'Key');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 11, 'Plug');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 12, 'Servo');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 13, 'LED Analog');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 14, 'External');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 15, 'Local');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 16, 'Xbee');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 17, 'Elettricity');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 18, 'Gas');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 19, 'Lux analog GA1A12S202');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 20, 'Lux analog Grove');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 21, 'Movement');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 22, 'Pressure');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 23, 'Noise');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 24, 'Temp DHT21');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 25, 'Temperature');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 26, 'External Temp');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 27, 'Umidity DH21');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 28, 'External Umidity');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 29, 'for internal use only');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 30, 'Always');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 31, 'Daily');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 32, 'Weekly');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 33, 'Monthly');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 34, 'On');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 35, 'Off');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 36, 'Local');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 37, 'Remote');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 38, 'Remote temp');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 39, 'Remoto FlipFlop');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 40, 'Remote Timer');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 41, 'Yes');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 42, 'No');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 43, 'Inactive');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 1, 'Attivo');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 2, 'Umidità');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 3, 'Illuminazione');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 4, 'Temperatura');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 5, 'Lampada');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 6, 'Lampadario');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 7, 'Relè');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 8, 'Cicalino');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 9, 'Videocamera');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 10, 'Chiave');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 11, 'Presa corrente');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 12, 'Servo');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 13, 'LED analogico');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 14, 'Esterno');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 15, 'Locale');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 16, 'Xbee');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 17, 'Elettricità');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 18, 'Gas');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 19, 'Lux analog GA1A12S202');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 20, 'Lux analog Grove');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 21, 'Movimento');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 22, 'Pressione');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 23, 'Rumore');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 24, 'Temp DHT21');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 25, 'Temperatura');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 26, 'Temperatura estrerna');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 27, 'Umidità DH21');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 28, 'Umidità esterna');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 29, 'non usare');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 30, 'Sempre');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 31, 'Giornaliero');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 32, 'Settimanale');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 33, 'Mensile');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 34, 'Acceso');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 35, 'Spento');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 36, 'Locale');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 37, 'Remoto permanente');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 38, 'Remoto temporaneo');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 39, 'Remoto FlipFlop');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 40, 'Remoto timer');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 41, 'Si');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 42, 'No');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 43, 'Disattivo');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 44, 'Suspended');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 44, 'Sospeso');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 100, 'If');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 100, 'Se');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 101, 'between');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 101, 'fra');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 102, 'and');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 102, 'e');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 103, 'or');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 103, 'o');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 104, 'then');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 104, 'allora');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 45, 'Away');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 45, 'Via');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 46, 'Back home');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 46, 'In casa');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 200, 'Systems');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 200, 'Sistemi');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 201, 'Functions');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 201, 'Funzioni');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 202, 'Favourites');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 202, 'Favoriti');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 47, 'Sockets');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 47, 'Prese');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 48, 'Door');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 48, 'Porta');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 49, 'Switch');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 49, 'Interruttore');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 50, 'Table lamp');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 50, 'Lampada da tavolo');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 51, 'Window');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 51, 'Finestra');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 52, 'Air conditioner');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 52, 'Condizionatore');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 53, 'Fan');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 53, 'Ventilatore');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 54, 'Mail');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 54, 'Posta');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 55, 'Menu');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 55, 'Menù');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 56, 'Step');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 56, 'Passo');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 57, 'Stop');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 57, 'Stop');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('EN', 58, 'Pollution');
INSERT INTO `dh`.`tbtranslationitem` (`tbLanguage_id`, `tbTranslation_id`, `wording`) VALUES ('IT', 58, 'Inquinamento');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbtriggertype`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbtriggertype` (`id`, `tbIcon_id`, `tbTranslation_id`) VALUES (0, 21, 36);
INSERT INTO `dh`.`tbtriggertype` (`id`, `tbIcon_id`, `tbTranslation_id`) VALUES (1, 22, 37);
INSERT INTO `dh`.`tbtriggertype` (`id`, `tbIcon_id`, `tbTranslation_id`) VALUES (2, 23, 38);
INSERT INTO `dh`.`tbtriggertype` (`id`, `tbIcon_id`, `tbTranslation_id`) VALUES (3, 24, 39);
INSERT INTO `dh`.`tbtriggertype` (`id`, `tbIcon_id`, `tbTranslation_id`) VALUES (4, 25, 40);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbtrigger`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbtrigger` (`id`, `tbTriggerType_id`, `tbEvent_id`, `tbActionGroup_id`, `tbStatus_id`, `tbTimer_id`, `currentstatus`, `sendalert`) VALUES (1, 3, 5, 1, 1, 1, 0, 0);
INSERT INTO `dh`.`tbtrigger` (`id`, `tbTriggerType_id`, `tbEvent_id`, `tbActionGroup_id`, `tbStatus_id`, `tbTimer_id`, `currentstatus`, `sendalert`) VALUES (2, 3, 6, 2, 1, 1, 0, 0);
INSERT INTO `dh`.`tbtrigger` (`id`, `tbTriggerType_id`, `tbEvent_id`, `tbActionGroup_id`, `tbStatus_id`, `tbTimer_id`, `currentstatus`, `sendalert`) VALUES (3, 3, 7, 6, 1, 1, 0, 0);
INSERT INTO `dh`.`tbtrigger` (`id`, `tbTriggerType_id`, `tbEvent_id`, `tbActionGroup_id`, `tbStatus_id`, `tbTimer_id`, `currentstatus`, `sendalert`) VALUES (4, 3, 8, 4, 1, 1, 0, 0);
INSERT INTO `dh`.`tbtrigger` (`id`, `tbTriggerType_id`, `tbEvent_id`, `tbActionGroup_id`, `tbStatus_id`, `tbTimer_id`, `currentstatus`, `sendalert`) VALUES (5, 3, 9, 5, 1, 1, 0, 0);
INSERT INTO `dh`.`tbtrigger` (`id`, `tbTriggerType_id`, `tbEvent_id`, `tbActionGroup_id`, `tbStatus_id`, `tbTimer_id`, `currentstatus`, `sendalert`) VALUES (6, 3, 10, 7, 1, 1, 0, 0);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbproctl`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbproctl` (`id`, `sts`, `proc`) VALUES (0, 0, 'updAll');
INSERT INTO `dh`.`tbproctl` (`id`, `sts`, `proc`) VALUES (1, 0, 'updateINData');
INSERT INTO `dh`.`tbproctl` (`id`, `sts`, `proc`) VALUES (101, 0, 'dh20.py');
INSERT INTO `dh`.`tbproctl` (`id`, `sts`, `proc`) VALUES (102, 0, 'getExtTemp.py');
INSERT INTO `dh`.`tbproctl` (`id`, `sts`, `proc`) VALUES (103, 0, 'scheduler.py');
INSERT INTO `dh`.`tbproctl` (`id`, `sts`, `proc`) VALUES (104, 0, 'scheduler.py');

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbfunction`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbfunction` (`id`, `descr`, `tbicon_id`, `currentstatus`) VALUES (1, 'Away/At home', 31, 0);
INSERT INTO `dh`.`tbfunction` (`id`, `descr`, `tbicon_id`, `currentstatus`) VALUES (2, 'Close/Open All', 32, 0);
INSERT INTO `dh`.`tbfunction` (`id`, `descr`, `tbicon_id`, `currentstatus`) VALUES (3, 'Night vision', 46, 0);

COMMIT;


-- -----------------------------------------------------
-- Data for table `dh`.`tbmenu`
-- -----------------------------------------------------
START TRANSACTION;
USE `dh`;
INSERT INTO `dh`.`tbmenu` (`page`, `tbicon_id`, `tbtranslation_id`) VALUES ('system.php', 36, 200);
INSERT INTO `dh`.`tbmenu` (`page`, `tbicon_id`, `tbtranslation_id`) VALUES ('function.php', 37, 201);
INSERT INTO `dh`.`tbmenu` (`page`, `tbicon_id`, `tbtranslation_id`) VALUES ('favourites.php', 38, 202);

COMMIT;

USE `dh`;

DELIMITER $$
USE `dh`$$
CREATE DEFINER=`root`@`localhost` TRIGGER `dh`.`tbTranslation_AINS`
AFTER INSERT ON `dh`.`tbtranslation`
FOR EACH ROW
BEGIN

insert into tbTranslationItem (tbLanguage_id,tbTranslation_id,wording)
select tbLanguage.id, NEW.id, 'insert value here'
from tbLanguage;

END$$


DELIMITER ;
