Module status **under development, not suitable for deployment**.

The datasets module provides forms for users to submit data.  This includes: 

* creating an organism, and waiting for admin approval before its created
* creating an analysis, and waiting for admin approval before its created 
  - module does not currently create analyses
  

To see this module in action, pleas visit https://i5k.nal.usda.gov/datasets/submit-a-dataset 

# Set up

### Patch the captcha module
1. Applied patch to captcha.module file due to erro "CAPTCHA session reuse attack detected"

https://www.drupal.org/files/issues/1395184_31.patch


### Configure your site

Make sure that the site name and email variables are set for your site, as these will be used in the email messages sent to admins and users.  You can check your configuration at `/admin/config/system/site-information`



### More

Steps: for New Organism
1. Request new organism If its not there in dropdown
   To request go to url: http://domain_url/datasets/request-project
2. Once the user submitted the request organism, an email goes to administrator.
3. Administrator checks and sees if the organism is worth to approve/reject.
4. An email goes to the requested user about the organism approval if it is approved. If rejected he wont  receives and email.

## Dependencies
the below modules will be installed when enabling datasets:

1. Date module
2. honeypot module


# Admin usage

Currently, if running a Tripal 3 site, organism records will be inserted into Chado after approving a request at `admin/structure/datasets` in the "Request Project List" tab.  They will not be available to users until they are **published**.  Visit `admin -> Content Types -> Tripal Content types -> Publish` after approving an organism to publish it.