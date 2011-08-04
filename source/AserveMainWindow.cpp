/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-7 by Raw Material Software ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   JUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with JUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
   Boston, MA 02111-1307 USA

  ------------------------------------------------------------------------------

   If you'd like to release a closed-source product which uses JUCE, commercial
   licenses are also available: visit www.rawmaterialsoftware.com/juce for
   more information.

  ==============================================================================
*/

#include <juce.h>
#include "AserveMainWindow.h"
#include "AserveComponent.h"

//==============================================================================


//==============================================================================
class ContentComp  : public Component
                     //public MenuBarModel,
                    // public ApplicationCommandTarget
{
    //==============================================================================
    MainDemoWindow* mainWindow;

    OldSchoolLookAndFeel oldLookAndFeel;

    Aserve* aserve;

    TooltipWindow tooltipWindow; // to add tooltips to an application, you
                                 // just need to create one of these and leave it
                                 // there to do its work..

    /*enum CommandIDs
    {
        showAudioSetup                  = 0x2005,
		showSampleChooser				= 0x2006,
		showSequencerWindow				= 0x2007
    };*/

public:
    //==============================================================================
    ContentComp (MainDemoWindow* mainWindow_) : mainWindow (mainWindow_), aserve (0)
    {
		//mainWindow->setUsingNativeTitleBar (! mainWindow->isUsingNativeTitleBar());
		aserve = new Aserve();
		addAndMakeVisible (aserve);
    }

    ~ContentComp()
    {
        // (need to do this because the old school look-and-feel object is one of our members,
        // so will be deleted with us, and would leave a dangling pointer if it's selected)
        LookAndFeel::setDefaultLookAndFeel (0);
		delete aserve;
        deleteAllChildren();
    }

    //==============================================================================
    void resized()
    {
        if (aserve != 0)
            aserve->setBounds (0, 0, getWidth(), getHeight());
    }
    //==============================================================================
    /*const StringArray getMenuBarNames()
    {
        const tchar* const names[] = { T("Aserve"), 0 };

        return StringArray ((const tchar**) names);
    }

    const PopupMenu getMenuForIndex (int menuIndex,
                                     const String& menuName)
    {
        ApplicationCommandManager* const commandManager = mainWindow->commandManager;

        PopupMenu menu;

        if (menuIndex == 0)
        {
			menu.addCommandItem (commandManager, showSampleChooser);
            menu.addCommandItem (commandManager, showSequencerWindow);
			menu.addSeparator();
			menu.addCommandItem (commandManager, showAudioSetup);
            menu.addSeparator();
            menu.addCommandItem (commandManager, StandardApplicationCommandIDs::quit);
        }

        return menu;
    }

    void menuItemSelected (int menuItemID,
                           int topLevelMenuIndex)
    {
        // all our menu items are invoked automatically as commands, so no need to do
        // anything in this callback
    }

    //==============================================================================
    // The following methods implement the ApplicationCommandTarget interface, allowing
    // this window to publish a set of actions it can perform, and which can be mapped
    // onto menus, keypresses, etc.

    ApplicationCommandTarget* getNextCommandTarget()
    {
        // this will return the next parent component that is an ApplicationCommandTarget (in this
        // case, there probably isn't one, but it's best to use this method in your own apps).
        return findFirstTargetParentComponent();
    }

    void getAllCommands (Array <CommandID>& commands)
    {
        // this returns the set of all commands that this target can perform..
        const CommandID ids[] = {	showAudioSetup, 
									showSampleChooser,
									showSequencerWindow
								};

        commands.addArray (ids, numElementsInArray (ids));
    }

    // This method is used when something needs to find out the details about one of the commands
    // that this object can perform..
    void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
    {
        const String generalCategory (T("General"));
        const String demosCategory (T("Demos"));

        switch (commandID)
        {
       
        case showAudioSetup:
            result.setInfo (T("Audio settings"), T("Displays audio settings"), demosCategory, 0);
            //result.setTicked (currentDemoId == showAudioSetup);
            result.addDefaultKeypress (T('1'), ModifierKeys::commandModifier);
            break;
				
		case showSampleChooser:
			result.setInfo (T("Sample Selector Window"), T("Displays sample selector window"), demosCategory, 0);
			//result.setTicked (currentDemoId == showAudioSetup);
			result.addDefaultKeypress (T('2'), ModifierKeys::commandModifier);
			break;
		
		case showSequencerWindow:
			result.setInfo (T("Sequencer Window"), T("Displays sequencer window"), demosCategory, 0);
			//result.setTicked (currentDemoId == showAudioSetup);
			result.addDefaultKeypress (T('3'), ModifierKeys::commandModifier);
			break;

				
        default:
            break;
        };
    }

    // this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
    bool perform (const InvocationInfo& info)
    {
        switch (info.commandID)
        {
			case showAudioSetup:
				aserve->ShowAudioSettings();
            break;
				
			case showSampleChooser:
				aserve->showSampleSelector();
			break;
				
			case showSequencerWindow:
				aserve->showSequencerWindow();
				break;

        default:
            return false;
        };

        return true;
    }
*/
    juce_UseDebuggingNewOperator
};

//==============================================================================
#if JUCE_WIN32 || JUCE_LINUX

// Just add a simple icon to the Window system tray area..
class DemoTaskbarComponent  : public SystemTrayIconComponent
{
public:
    DemoTaskbarComponent()
    {
        // Create an icon which is just a square with a "j" in it..
        Image icon (Image::RGB, 32, 32, true);
        Graphics g (icon);
        g.fillAll (Colours::lightblue);
        g.setColour (Colours::black);
        g.setFont ((float) icon.getHeight(), Font::bold);
        g.drawText (T("j"), 0, 0, icon.getWidth(), icon.getHeight(), Justification::centred, false);

        setIconImage (icon);

        setIconTooltip (T("Aserve"));
    }

    ~DemoTaskbarComponent()
    {
    }

    void mouseDown (const MouseEvent& e)
    {
        PopupMenu m;
        m.addItem (1, T("Quit Aserve"));

        const int result = m.show();

        if (result == 1)
            JUCEApplication::getInstance()->systemRequestedQuit();
    }
};

#endif

//==============================================================================
MainDemoWindow::MainDemoWindow()
    : DocumentWindow (T("Aserve - v1.3"),
                      Colours::lightgrey,
                      DocumentWindow::allButtons,
                      true)
{
    //commandManager = new ApplicationCommandManager();

    setResizable (true, false); // resizability is a property of ResizableWindow

    ContentComp* contentComp = new ContentComp (this);

    //commandManager->registerAllCommandsForTarget (contentComp);
    //commandManager->registerAllCommandsForTarget (JUCEApplication::getInstance());

    // this lets the command manager use keypresses that arrive in our window to send
    // out commands
    //addKeyListener (commandManager->getKeyMappings());

    // sets the main content component for the window to be this tabbed
    // panel. This will be deleted when the window is deleted.
    setContentComponent (contentComp);

    // this tells the DocumentWindow to automatically create and manage a MenuBarComponent
    // which uses our contentComp as its MenuBarModel
    //setMenuBar (contentComp);

    // tells our menu bar model that it should watch this command manager for
    // changes, and send change messages accordingly.
    //contentComp->setApplicationCommandManagerToWatch (commandManager);
	
    setVisible (true);
	

#if JUCE_WIN32 || JUCE_LINUX
    taskbarIcon = new DemoTaskbarComponent();
#endif
}

MainDemoWindow::~MainDemoWindow()
{
#if JUCE_WIN32 || JUCE_LINUX
    deleteAndZero (taskbarIcon);
#endif

    // because we've set the content comp to be used as our menu bar model, we
    // have to switch this off before deleting the content comp..
    setMenuBar (0);

    // setting our content component to 0 will delete the current one, and
    // that will in turn delete all its child components. You don't always
    // have to do this explicitly, because the base class's destructor will
    // also delete the content component, but in this case we need to
    // make sure our content comp has gone away before deleting our command
    // manager.
    setContentComponent (0, true);

    //delete commandManager;
}

void MainDemoWindow::closeButtonPressed()
{
    // The correct thing to do when you want the app to quit is to call the
    // JUCEApplication::systemRequestedQuit() method.

    // That means that requests to quit that come from your own UI, or from other
    // OS-specific sources (e.g. the dock menu on the mac) all get handled in the
    // same way.

    JUCEApplication::getInstance()->systemRequestedQuit();
}
